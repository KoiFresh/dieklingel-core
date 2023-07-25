package config

import (
	"log"
	"os"

	"github.com/KoiFresh/dieklingel-core/core/internal/models"
	"gopkg.in/yaml.v3"
)

type Config struct {
	Actions []models.Action `yaml:"actions"`
	Mqtt    struct {
		Uri      string `yaml:"uri"`
		Username string `yaml:"username"`
		Password string `yaml:"password"`
	} `yaml:"mqtt"`
	Pins []models.Sensor `yaml:"pins"`
}

func NewConfigFromCurrentDirectory() (*Config, error) {
	data, err := os.ReadFile("core.yaml")
	if err != nil {
		log.Printf("cannot read config file: %s", err.Error())
		return nil, err
	}

	config := &Config{}

	if err := yaml.Unmarshal(data, &config); err != nil {
		log.Printf("cannot parse config file: %s", err.Error())
		return nil, err
	}

	return config, nil
}
