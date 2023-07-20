package handlers

import (
	"encoding/json"
	"log"
	"path"

	"github.com/dieklingel-core/core"
	mqtt "github.com/eclipse/paho.mqtt.golang"
)

type HandlerFunction func(core.Request) core.Response

func register(client mqtt.Client, channel string, handler HandlerFunction) {
	client.Subscribe(channel, 2, func(c mqtt.Client, m mqtt.Message) {
		var request = core.Request{}

		if err := json.Unmarshal(m.Payload(), &request); err != nil {
			log.Printf("Could not parse the request: %s. Request will be silently ignored.\r\n", err.Error())
			return
		}

		var response = handler(request)
		if request.IsSocketMessage() {
			return
		}

		json, err := json.Marshal(response)
		if err != nil {
			log.Printf("Could not parse the response: %s. Response will no be sent to the remote.\r\n", err.Error())
			return
		}

		answerChannel, err := request.GetAnswerChannel()
		if err != nil {
			log.Printf("Could not parse the answer channel: %s. Response will no be sent to the remote.\r\n", err.Error())
			return
		}

		client.Publish(path.Join(channel, answerChannel), 2, false, string(json))
	})
}
