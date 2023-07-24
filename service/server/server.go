package server

import (
	"log"
	"net/url"
	"path"
	"time"

	"github.com/dieklingel-core/handlers"
	mqtt "github.com/eclipse/paho.mqtt.golang"
	"github.com/google/uuid"
)

func Run(url url.URL, username string, password string) {
	var id = uuid.New()
	log.Printf("Start connection with id '%s'.\r\n", id.String())
	prefix := path.Join("./", url.Path)

	var options = mqtt.NewClientOptions()
	options.AddBroker(url.String())
	options.SetClientID(id.String())
	options.SetUsername(username)
	options.SetPassword(password)
	options.SetAutoReconnect(true)
	options.SetKeepAlive(10 * time.Second)
	options.OnConnect = onConnect
	options.OnConnectionLost = onConnectionLost

	var client = mqtt.NewClient(options)

	for !client.IsConnected() {
		if token := client.Connect(); token.Wait() && token.Error() != nil {
			log.Printf("Could not connect tho the broker. Message: %s\r\n", token.Error().Error())
			log.Printf("Retry connecting in 10 seconds.\r\n")
			time.Sleep(10 * time.Second)
		}
	}

	// TODO: use prefix from env or input
	handlers.RegisterCameraHandler(prefix+"/camera", client)
	handlers.RegisterRtcHandler(prefix+"/rtc", client)
	handlers.RegisterDeviceHandler(prefix+"/devices", client)
	handlers.RegisterActionHandler(prefix+"/actions", client)
	handlers.RegisterSignHandler(prefix+"/signs", client)
}

func onConnect(client mqtt.Client) {
	log.Printf("Connected to the broker.\r\n")
}

func onConnectionLost(client mqtt.Client, err error) {
	log.Printf("Connection lost.\r\n")
}
