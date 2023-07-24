package main

import (
	"log"
	"net/url"
	"os"
	"os/signal"
	"syscall"

	"github.com/dieklingel-core/core"
	"github.com/dieklingel-core/server"
)

func main() {
	config, err := core.NewConfigFromCurrentDirectory()
	if err != nil {
		log.Printf("cannot read config fiel: %s", err.Error())
		os.Exit(1)
	}

	uri, err := url.Parse(config.Mqtt.Uri)
	if err != nil {
		log.Printf("cannot parse mqtt uri: %s", err.Error())
		os.Exit(1)
	}

	server.Run(
		*uri,
		config.Mqtt.Password,
		config.Mqtt.Username,
	)

	// Wait for interruption to exit
	var sigint = make(chan os.Signal, 1)
	signal.Notify(sigint, os.Interrupt, syscall.SIGTERM)
	<-sigint
}
