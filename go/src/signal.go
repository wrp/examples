
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
	quit := make(chan os.Signal)
	signal.Notify(quit, os.Interrupt)
	var wg sync.WaitGroup
	handle_interrupts(&wg, quit)

	wg.Wait()
}
