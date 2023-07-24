package core

import (
	"log"
	"os"

	"gopkg.in/yaml.v3"
)

type Config struct {
	Actions []Action `yaml:"actions"`
	Mqtt    struct {
		Uri      string `yaml:"uri"`
		Username string `yaml:"username"`
		Password string `yaml:"password"`
	} `yaml:"mqtt"`
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