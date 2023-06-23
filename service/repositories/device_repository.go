package repositories

import (
	"github.com/dieklingel-core/models"
)

var Devices *DeviceRepository = &DeviceRepository{}

type DeviceRepository struct{}

func (repository *DeviceRepository) FetchAllDevices() []models.Device {
	var list = make([]models.Device, 0)

	return list
}
