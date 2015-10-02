package main

import (
	"flag"
	"log"
	"os"
	"os/signal"
	"sync"
	"time"

	"github.com/oleksandr/bonjour"
)

func main() {
	flag.Parse()
	if !*server && !*client {
		log.Println("must run either in server or client mode")
		os.Exit(1)
	}
	if *server {
		if len(*folder) == 0 {
			log.Println("must specify folder")
			os.Exit(1)
		}
		isThere, err := exists(*folder)
		if err != nil {
			log.Println(err)
			os.Exit(1)
		}
		if !isThere {
			log.Println("folder", *folder, "does not exist")
			os.Exit(1)
		}
		if err := registerService(); err != nil {
			log.Println(err)
			os.Exit(1)
		}
	}
	if *client {
		if err := browseServices(); err != nil {
			log.Println(err)
			os.Exit(1)
		}
	}
	os.Exit(0)
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
)

type service struct {
	hostName string
	port     int
	m        sync.Mutex
}

func (s *service) update(entry bonjour.ServiceEntry) {
	s.m.Lock()
	defer s.m.Unlock()
	s.hostName = entry.HostName
	s.port = entry.Port
}

var currentService service

const animationServiceName = "animation_server"
const port = 8000

func registerService() error {
	// Run registration (blocking call)
	s, err := bonjour.Register("animation_server", "_foobar._tcp", "", 9999, []string{"txtv=1", "app=test"}, nil)
	if err != nil {
		return err
	}

	// Ctrl+C handling
	handler := make(chan os.Signal, 1)
	signal.Notify(handler, os.Interrupt)
	for sig := range handler {
		if sig == os.Interrupt {
			s.Shutdown()
			time.Sleep(1e9)
			break
		}
	}

	return nil
}

func browseServices() error {
	log.Println("browsing services..")

	resolver, err := bonjour.NewResolver(nil)
	if err != nil {
		log.Println("Failed to initialize resolver:", err.Error())
		return err
	}

	results := make(chan *bonjour.ServiceEntry)

	go func(results chan *bonjour.ServiceEntry, exitCh chan<- bool) {
		for e := range results {
			log.Printf("found service %s", e.Instance)
		}
	}(results, resolver.Exit)

	err = resolver.Browse("_foobar._tcp", "local.", results)
	if err != nil {
		log.Println("Failed to browse:", err.Error())
	}

	select {}
}
