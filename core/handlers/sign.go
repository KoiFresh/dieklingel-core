package handlers

import (
	"path"
	"strings"

	"github.com/KoiFresh/dieklingel-core/core/internal/action"
	"github.com/KoiFresh/dieklingel-core/core/internal/handler"
	"github.com/KoiFresh/dieklingel-core/core/internal/models"
	mqtt "github.com/eclipse/paho.mqtt.golang"
)

func RegisterSignHandler(prefix string, client mqtt.Client) {
	handler.Register(client, path.Join(prefix, "ring", "+"), onSignRing)
}

func onSignRing(client mqtt.Client, req models.Request) models.Response {
	pathSegments := strings.Split(req.RequestPath, "/")
	sign := pathSegments[len(pathSegments)-1]

	action.ExecuteActionsFromPattern(
		"ring",
		map[string]string{
			"SIGN": sign,
		},
	)

	return models.NewResponse("Ok", 200)
}
