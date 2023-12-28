
package main

import (
	"log"
	"os"
	"os/signal"
	"sync"
)

func main() {
	quit := make(chan os.Signal)
	signal.Notify(quit, os.Interrupt)
	var wg sync.WaitGroup

	wg.Add(1)
	go func() {
		defer wg.Done()
		<-quit
		log.Println("received interrupt signal")
	}()

	wg.Wait()
}
