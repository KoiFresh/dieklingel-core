package sensor

import (
	"log"

	"github.com/KoiFresh/dieklingel-core/core/internal/action"
	"github.com/KoiFresh/dieklingel-core/core/internal/models"
	"github.com/stianeikeland/go-rpio/v4"
)

func RunListener(sensors []models.Sensor) {
	go func() {
		if err := rpio.Open(); err != nil {
			log.Printf("cannot open gpio: %s", err.Error())
			return
		}

		for {
			for _, sensor := range sensors {
				run(sensor)
			}
		}
	}()
}

func run(s models.Sensor) {
	state := rpio.ReadPin(rpio.Pin(s.Pin))

	switch {
	case s.ReadType == models.High && state == 1:
		action.ExecuteActionsFromPattern(s.Trigger, make(map[string]string))
	case s.ReadType == models.Low && state == 0:
		action.ExecuteActionsFromPattern(s.Trigger, make(map[string]string))
	case s.ReadType == models.PositiveEdge && state == 1 && s.State == 0:
		action.ExecuteActionsFromPattern(s.Trigger, make(map[string]string))
	case s.ReadType == models.NegativeEdge && state == 0 && s.State == 1:
		action.ExecuteActionsFromPattern(s.Trigger, make(map[string]string))
	}

	s.State = state
}
