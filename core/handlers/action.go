package handlers

import (
	"encoding/json"
	"fmt"
	"log"
	"os/exec"
	"path"
	"regexp"

	"github.com/KoiFresh/dieklingel-core/core/models"
	mqtt "github.com/eclipse/paho.mqtt.golang"
)

func RegisterActionHandler(prefix string, client mqtt.Client) {
	register(client, path.Join(prefix), onActions)
	register(client, path.Join(prefix, "execute"), onExecuteActions)
}

func onActions(client mqtt.Client, req models.Request) models.Response {
	config, err := models.NewConfigFromCurrentDirectory()
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

	actions := ExecuteActions(pattern, environment)
	json, _ := json.Marshal(actions)
	return models.NewResponse(string(json), 200)
}

func ExecuteActions(pattern string, environment map[string]string) []models.Action {
	config, err := models.NewConfigFromCurrentDirectory()
	if err != nil {
		log.Printf("could not execute actions: %s", err.Error())
		return make([]models.Action, 0)
	}

	regex, err := regexp.Compile(pattern)
	if err != nil {
		log.Printf("error while compiling regex: %s", err.Error())
		return make([]models.Action, 0)
	}

	actions := make([]models.Action, 0)
	for _, action := range config.Actions {
		match := regex.MatchString(action.Trigger)
		if match {
			actions = append(actions, action)
			if err := exec.Command("bash", "-c", action.Lane).Run(); err != nil {
				log.Printf("error while running action: %s", err.Error())
			}
		}
	}

	return actions
}
