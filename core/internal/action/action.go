package action

import (
	"log"
	"os/exec"
	"regexp"

	"github.com/KoiFresh/dieklingel-core/core/internal/config"
	"github.com/KoiFresh/dieklingel-core/core/internal/models"
)

func ExecuteActionsFromPattern(pattern string, environment map[string]string) []models.Action {
	config, err := config.NewConfigFromCurrentDirectory()
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
