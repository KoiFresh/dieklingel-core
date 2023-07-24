package main

import (
	"os"
	"os/signal"
	"syscall"

	"github.com/dieklingel-core/server"
)

func main() {
	server.Run()

	// Wait for interruption to exit
	var sigint = make(chan os.Signal, 1)
	signal.Notify(sigint, os.Interrupt, syscall.SIGTERM)
	<-sigint
}
