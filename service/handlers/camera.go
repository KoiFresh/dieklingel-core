package handlers

import (
	"path"

	"github.com/dieklingel-core/core"
	mqtt "github.com/eclipse/paho.mqtt.golang"
)

func RegisterCameraHandler(prefix string, client mqtt.Client) {
	print("cam")
	register(client, path.Join(prefix, "snapshot"), onSnapshot)

	// TOOD: implement camera handler
}

func onSnapshot(req core.Request) core.Response {

	return core.NewEmptyResponse()
}
