package controllers

import (
	"github.com/dieklingel-core/repositories"
	"github.com/gin-gonic/gin"
)

type DeviceController struct {
	deviceRepository repositories.DeviceRepository
}

func NewDeviceController() *DeviceController {
	return &DeviceController{}
}

func (controller *DeviceController) All(context *gin.Context) {

	context.IndentedJSON(200, controller.deviceRepository.FetchAllDevices())
}
