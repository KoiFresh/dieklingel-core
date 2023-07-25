package models

import "github.com/stianeikeland/go-rpio/v4"

type ReadType string

const (
	High         ReadType = "high"
	Low          ReadType = "low"
	PositiveEdge ReadType = "pe"
	NegativeEdge ReadType = "ne"
)

type Sensor struct {
	Pin      int      `yaml:"pin"`
	Trigger  string   `yaml:"trigger"`
	ReadType ReadType `yaml:"read-type"`
	State    rpio.State
}
