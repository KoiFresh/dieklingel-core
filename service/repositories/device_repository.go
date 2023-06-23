package repositories

import (
	"github.com/dieklingel-core/models"
)

type DeviceRepository struct{}

func NewDeviceRepository() *DeviceRepository {
	return &DeviceRepository{}
}

func (repository *DeviceRepository) FetchAllDevices() []models.Device {
	var list = make([]models.Device, 0)

	return list
}
