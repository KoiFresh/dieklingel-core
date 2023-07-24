package handlers

import (
	"path"
	"strings"

	"github.com/dieklingel-core/core"
	mqtt "github.com/eclipse/paho.mqtt.golang"
)

func RegisterSignHandler(prefix string, client mqtt.Client) {
	register(client, path.Join(prefix, "ring", "+"), onSignRing)
}

func onSignRing(client mqtt.Client, req core.Request) core.Response {
	pathSegments := strings.Split(req.RequestPath, "/")
	sign := pathSegments[len(pathSegments)-1]

	ExecuteActions(
		"ring",
		map[string]string{
			"SIGN": sign,
		},
	)

	return core.NewResponse("Ok", 200)
}
