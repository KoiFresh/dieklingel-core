package handlers

import (
	"encoding/json"
	"fmt"
	"path"

	"github.com/KoiFresh/dieklingel-core/core/internal/action"
	"github.com/KoiFresh/dieklingel-core/core/internal/config"
	"github.com/KoiFresh/dieklingel-core/core/internal/handler"
	"github.com/KoiFresh/dieklingel-core/core/internal/models"
	mqtt "github.com/eclipse/paho.mqtt.golang"
)

func RegisterActionHandler(prefix string, client mqtt.Client) {
	handler.Register(client, path.Join(prefix), onActions)
	handler.Register(client, path.Join(prefix, "execute"), onExecuteActions)
}

func onActions(client mqtt.Client, req models.Request) models.Response {
	config, err := config.NewConfigFromCurrentDirectory()
	if err != nil {
		return models.NewResponse(fmt.Sprintf("Could not read config: %s", err.Error()), 500)
	}

	json, err := json.Marshal(config.Actions)
	if err != nil {
		return models.NewResponse(fmt.Sprintf("Could not serialize actions: %s", err.Error()), 500)
	}

	return models.NewResponse(string(json), 200)
}

func onExecuteActions(client mqtt.Client, req models.Request) models.Response {
	payload := make(map[string]interface{})
	json.Unmarshal([]byte(req.Body), &payload)

	pattern, ok := payload["pattern"].(string)
	if !ok {
		return models.NewResponse("the pattern has to be of type string", 400)
	}

	env, ok := payload["environment"].(map[string]interface{})
	if !ok {
		return models.NewResponse("the evironment has to be of type {string: string}", 400)
	}
	environment := make(map[string]string)
	for key, value := range env {
		environment[key] = fmt.Sprintf("%s", value)
	}

	actions := action.ExecuteActionsFromPattern(pattern, environment)
	json, _ := json.Marshal(actions)
	return models.NewResponse(string(json), 200)
}
