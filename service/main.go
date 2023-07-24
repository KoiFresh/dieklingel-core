package main

import (
	"log"
	"os"
	"os/signal"
	"syscall"
	"time"

	"github.com/dieklingel-core/handlers"
	mqtt "github.com/eclipse/paho.mqtt.golang"
	"github.com/google/uuid"
)

func main() {
	var id = uuid.New()
	log.Printf("Start connection with id '%s'.\r\n", id.String())

	var options = mqtt.NewClientOptions()
	options.AddBroker("mqtt://server.dieklingel.com:1883")
	options.SetClientID(id.String())
	options.SetUsername("admin")
	options.SetPassword("admin")
	options.SetAutoReconnect(true)
	options.SetKeepAlive(10 * time.Second)
	options.OnConnect = onConnect
	options.OnConnectionLost = onConnectionLost

	var client = mqtt.NewClient(options)

	for !client.IsConnected() {
		if token := client.Connect(); token.Wait() && token.Error() != nil {
			log.Printf("Could not connect tho the broker. Message: %s\r\n", token.Error().Error())
			log.Printf("Retry connecting in ten seconds.\r\n")
			time.Sleep(10 * time.Second)
		}
	}

	var prefix = "dieklingel/test/"

	// TODO: use prefix from env or input
	handlers.RegisterCameraHandler(prefix+"/camera", client)
	handlers.RegisterRtcHandler(prefix+"/rtc", client)

	// Wait for interruption to exit
	var sigint = make(chan os.Signal, 1)
	signal.Notify(sigint, os.Interrupt, syscall.SIGTERM)
	<-sigint
}

func onConnect(client mqtt.Client) {
	log.Printf("Connected to the broker.\r\n")
}

func onConnectionLost(client mqtt.Client, err error) {
	log.Printf("Connection lost.\r\n")
}
