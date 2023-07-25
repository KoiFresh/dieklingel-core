package main

import (
	"log"
	"net/url"
	"os"
	"os/signal"
	"strings"
	"syscall"

	"github.com/KoiFresh/dieklingel-core/core/internal/proxy"
	"github.com/KoiFresh/dieklingel-core/core/models"
	"github.com/KoiFresh/dieklingel-core/core/server"
)

func main() {
	wd := os.Getenv("DIEKLINGEL_HOME")
	if len(strings.TrimSpace(wd)) == 0 {
		log.Printf("the working directory cannot be empty. Please set the environment variable DIEKLINGEL_HOME")
		os.Exit(1)
	}
	if err := syscall.Chdir(wd); err != nil {
		log.Printf("error while switching to workdir: %s", err.Error())
		os.Exit(1)
	}
	dir, _ := syscall.Getwd()
	log.Printf("Running in working directory: %s", dir)

	config, err := models.NewConfigFromCurrentDirectory()
	if err != nil {
		log.Printf("cannot read config fiel: %s", err.Error())
		os.Exit(1)
	}

	uri, err := url.Parse(config.Mqtt.Uri)
	if err != nil {
		log.Printf("cannot parse mqtt uri: %s", err.Error())
		os.Exit(1)
	}

	server.RunListener(
		*uri,
		config.Mqtt.Password,
		config.Mqtt.Username,
	)
	proxy.RunListener(8081)

	// Wait for interruption to exit
	var sigint = make(chan os.Signal, 1)
	signal.Notify(sigint, os.Interrupt, syscall.SIGTERM)
	<-sigint
}
