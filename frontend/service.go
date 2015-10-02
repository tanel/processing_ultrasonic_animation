package main

import (
	"flag"
	"fmt"
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
	if *server && *client {
		log.Println("cannot run both in server and client mode")
		os.Exit(1)
	}
	s := &service{
		Name: "animation_server",
		Port: 8000,
	}
	if *server {
		log.Println("running server")
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
		if err := s.register(); err != nil {
			log.Println(err)
			os.Exit(1)
		}
	}
	if *client {
		log.Println("running client")
		if err := s.browseUpdates(); err != nil {
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
	Name     string
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
	return fmt.Sprintf("%s %s:%d", s.Name, s.HostName, s.Port)
}

func (s *service) register() error {
	// Run registration (blocking call)
	bonjourService, err := bonjour.Register(s.Name, "_foobar._tcp", "", s.Port, []string{"txtv=1", "app=test"}, nil)
	if err != nil {
		return err
	}

	// Ctrl+C handling
	handler := make(chan os.Signal, 1)
	signal.Notify(handler, os.Interrupt)
	for sig := range handler {
		if sig == os.Interrupt {
			bonjourService.Shutdown()
			time.Sleep(1e9)
			break
		}
	}

	return nil
}

func (s *service) browseUpdates() error {
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
			if s.Name == e.Instance {
				s.update(e)
				log.Println("updated", s)
			}
		}
	}(results, resolver.Exit)

	err = resolver.Browse("_foobar._tcp", "local.", results)
	if err != nil {
		log.Println("Failed to browse:", err.Error())
	}

	select {}
}
