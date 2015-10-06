package main

import (
	"flag"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"os"
	"path/filepath"
	"sync"
	"time"

	"github.com/gorilla/context"
	"github.com/gorilla/mux"
	"github.com/oleksandr/bonjour"
)

const serviceName = "animation_server"

func main() {
	flag.Parse()
	if !*server && !*client {
		log.Println("must run either in server or client mode")
		os.Exit(1)
	}
	if *server && *client {
		log.Println("cannot run both in server and client mode")
		os.Exit(1)
	}
	s := &service{}
	if *server {
		log.Println("running server on port", *port)
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
		if err := s.register(); err != nil {
			log.Println("error registering service", err)
			os.Exit(1)
		}
	}
	if *client {
		log.Println("running client on port", *port)
		go func() {
			if err := s.browseUpdates(); err != nil {
				log.Println("error browsing updates", err)
				os.Exit(1)
			}
		}()
		if err := s.proxyRequests(); err != nil {
			log.Println("error proxying requests", err)
			os.Exit(1)
		}
	}
	os.Exit(0)
}

var cacheFilename = filepath.Join("/", "tmp", "cached_gamestats.json")

func (s *service) fetchData() ([]byte, error) {
	resp, err := http.Get(fmt.Sprintf("http://%s:%d/gamestats.json", s.HostName, s.Port))
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

func (s *service) handlerFunc(w http.ResponseWriter, r *http.Request) {
	log.Println("serving request")
	b, err := s.fetchData()
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

func (s *service) proxyRequests() error {
	router := mux.NewRouter()
	router.HandleFunc("/gamestats.json", s.handlerFunc).Methods("GET", "OPTIONS")
	return http.ListenAndServe(fmt.Sprintf(":%d", *port), context.ClearHandler(router))
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
	server = flag.Bool("server", false, "serve data file over HTTP and advertise the service")
	folder = flag.String("folder", ".", "folder that includes gamestats.json")
	client = flag.Bool("client", false, "find where HTTP data is served and proxy it to localhost")
	port   = flag.Int("port", 8000, "port to listen on")
)

type service struct {
	HostName string
	Port     int
	m        sync.Mutex
}

func (s *service) update(entry *bonjour.ServiceEntry) {
	s.m.Lock()
	defer s.m.Unlock()
	s.HostName = entry.HostName
	s.Port = entry.Port
}

func (s *service) String() string {
	return fmt.Sprintf("%s:%d", s.HostName, s.Port)
}

func (s *service) register() error {
	// Run registration (blocking call)
	bonjourService, err := bonjour.Register(serviceName, "_foobar._tcp", "", *port, []string{"txtv=1", "app=test"}, nil)
	if err != nil {
		return err
	}
	defer bonjourService.Shutdown()
	h := http.FileServer(http.Dir(*folder))
	return http.ListenAndServe(fmt.Sprintf(":%d", *port), h)
}

func (s *service) browseUpdates() error {
	log.Println("resolving")

	resolver, err := bonjour.NewResolver(nil)
	if err != nil {
		log.Println("Failed to initialize resolver:", err.Error())
		return err
	}

	results := make(chan *bonjour.ServiceEntry)

	go func(results chan *bonjour.ServiceEntry, exitCh chan<- bool) {
		for e := range results {
			log.Printf("found service %s", e.Instance)
			if serviceName == e.Instance {
				s.update(e)
				log.Println("updated", s)
			}
		}
	}(results, resolver.Exit)

	go func() {
		for {
			err := resolver.Browse("_foobar._tcp", "local.", results)
			if err != nil {
				log.Println("Failed to browse:", err.Error())
			}
			time.Sleep(1 * time.Second)
		}
	}()

	select {}
}
