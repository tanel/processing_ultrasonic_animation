package main

import (
	"errors"
	"flag"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"net/http/pprof"
	"os"
	"path/filepath"

	"github.com/gorilla/context"
	"github.com/gorilla/mux"
)

func main() {
	flag.Parse()
	if len(*folder) == 0 {
		log.Println("must specify folder")
		os.Exit(1)
	}
	isThere, err := exists(*folder)
	if err != nil {
		log.Println("error checking folder", err)
		os.Exit(1)
	}
	if !isThere {
		log.Println("folder", *folder, "does not exist")
		os.Exit(1)
	}
	logfilePath := filepath.Join(*folder, "service.log")
	f, err := os.OpenFile(logfilePath, os.O_RDWR|os.O_CREATE|os.O_APPEND, 0666)
	if err != nil {
		log.Println("error opening log file", logfilePath, err)
		os.Exit(1)
	}
	defer f.Close()
	log.SetOutput(f)
	if !*server && !*client {
		log.Println("must run either in server or client mode")
		os.Exit(1)
	}
	if *server && *client {
		log.Println("cannot run both in server and client mode")
		os.Exit(1)
	}
	if *server {
		log.Println("running server on port", *port)
		h := http.FileServer(http.Dir(*folder))
		err := http.ListenAndServe(fmt.Sprintf(":%d", *port), h)
		if err != nil {
			log.Println("error serving", err)
			os.Exit(1)
		}
	}
	if *client {
		if len(*backendHost) == 0 {
			log.Println("missing backend host")
			os.Exit(1)
		}
		if *backendPort == 0 {
			log.Println("missing backend port")
			os.Exit(1)
		}
		log.Println("running client on port", *port)
		router := mux.NewRouter()
		attachProfiler(router)
		router.Handle("/gamestats.json", recoverWrap(http.HandlerFunc(handlerFunc))).Methods("GET", "OPTIONS")
		err := http.ListenAndServe(fmt.Sprintf(":%d", *port), context.ClearHandler(router))
		if err != nil {
			log.Println("error proxying requests", err)
			os.Exit(1)
		}
	}
	os.Exit(0)
}

var cacheFilename = filepath.Join("/", "tmp", "cached_gamestats.json")

func fetchData() ([]byte, error) {
	resp, err := http.Get(fmt.Sprintf("http://%s:%d/gamestats.json", *backendHost, *backendPort))
	if err != nil {
		return nil, err
	}
	defer resp.Body.Close()
	b, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		return nil, err
	}
	if err := ioutil.WriteFile(cacheFilename, b, 0644); err != nil {
		return nil, err
	}
	return b, nil
}

func handlerFunc(w http.ResponseWriter, r *http.Request) {
	log.Println("serving request")
	b, err := fetchData()
	if err != nil {
		log.Println("error fetching data", err)
		b, err = ioutil.ReadFile(cacheFilename)
		if err != nil {
			log.Println("error reading cached file", err)
		}
		log.Println("backend is unreachable, serving cache")
	}
	w.Header().Add("Access-Control-Allow-Origin", "*")
	w.Header().Add("Access-Control-Allow-Headers", "Cache-Control, Pragma, Origin, Authorization, Content-Type, X-Requested-With")
	w.Header().Add("Access-Control-Allow-Methods", "GET, OPTIONS, PUT, POST, DELETE")
	w.Header().Add("Access-Control-Allow-Credentials", "true")
	w.Header().Set("Content-Type", "application/json")
	_, err = w.Write(b)
	if err != nil {
		log.Println("error writing data to client", err)
	}
}

func attachProfiler(router *mux.Router) {
	router.HandleFunc("/debug/pprof/", pprof.Index)
	router.HandleFunc("/debug/pprof/cmdline", pprof.Cmdline)
	router.HandleFunc("/debug/pprof/profile", pprof.Profile)
	router.HandleFunc("/debug/pprof/symbol", pprof.Symbol)
}

// exists returns whether the given file or directory exists or not
func exists(path string) (bool, error) {
	_, err := os.Stat(path)
	if err == nil {
		return true, nil
	}
	if os.IsNotExist(err) {
		return false, nil
	}
	return true, err
}

var (
	server      = flag.Bool("server", false, "serve data file over HTTP and advertise the service")
	folder      = flag.String("folder", "", "folder that includes gamestats.json")
	client      = flag.Bool("client", false, "find where HTTP data is served and proxy it to localhost")
	port        = flag.Int("port", 8000, "port to listen on")
	backendPort = flag.Int("backend_port", 9000, "backend service is assumed to run on this port")
	backendHost = flag.String("backend_host", "", "backend service is assumed to run on this host")
)

func recoverWrap(h http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		var err error
		defer func() {
			r := recover()
			if r != nil {
				switch t := r.(type) {
				case string:
					err = errors.New(t)
				case error:
					err = t
				default:
					err = errors.New("Unknown error")
				}
				log.Println("panic", err)
				http.Error(w, err.Error(), http.StatusInternalServerError)
			}
		}()
		h.ServeHTTP(w, r)
	})
}
