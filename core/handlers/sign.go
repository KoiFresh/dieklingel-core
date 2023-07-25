package handlers

import (
	"path"
	"strings"

	"github.com/KoiFresh/dieklingel-core/core/models"
	mqtt "github.com/eclipse/paho.mqtt.golang"
)

func RegisterSignHandler(prefix string, client mqtt.Client) {
	register(client, path.Join(prefix, "ring", "+"), onSignRing)
}

func onSignRing(client mqtt.Client, req models.Request) models.Response {
	pathSegments := strings.Split(req.RequestPath, "/")
	sign := pathSegments[len(pathSegments)-1]

	ExecuteActions(
		"ring",
		map[string]string{
			"SIGN": sign,
		},
	)

	return models.NewResponse("Ok", 200)
}
