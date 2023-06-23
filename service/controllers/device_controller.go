package controllers

import (
	"github.com/dieklingel-core/repositories"
	"github.com/gin-gonic/gin"
)

var Devices *DeviceController = &DeviceController{}

type DeviceController struct{}

func (controller *DeviceController) All(context *gin.Context) {

	context.IndentedJSON(200, repositories.Devices.FetchAllDevices())
}
