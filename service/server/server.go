package server

import (
	"fmt"

	"github.com/dieklingel-core/controllers"
	"github.com/gin-gonic/gin"
)

func Init(port int16) {
	var router = gin.New()

	var deviceController = controllers.NewDeviceController()
	var devicesRoute = router.Group("/devices")
	{
		devicesRoute.GET("/", deviceController.All)
	}

	if err := router.Run(fmt.Sprintf(":%d", port)); err != nil {
		fmt.Println(err)
	}
}
