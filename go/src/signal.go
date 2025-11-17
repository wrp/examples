
package main

import (
	"log"
	"os"
	"os/signal"
	"sync"
)

func handle_interrupts(wg * sync.WaitGroup, quit chan os.Signal){
	wg.Add(1)
	go func() {
		defer wg.Done()
		<-quit
		log.Println("received interrupt signal")
	}()
}


func main() {
	// Use buffered channel to avoid missing signals
	quit := make(chan os.Signal, 1)
	signal.Notify(quit, os.Interrupt)
	var wg sync.WaitGroup
	handle_interrupts(&wg, quit)

	wg.Wait()
}
