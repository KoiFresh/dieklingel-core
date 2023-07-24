package handlers

import (
	"encoding/hex"
	"fmt"
	"path"
	"time"

	"github.com/dieklingel-core/core"
	mqtt "github.com/eclipse/paho.mqtt.golang"
	"gocv.io/x/gocv"
)

func RegisterCameraHandler(prefix string, client mqtt.Client) {
	register(client, path.Join(prefix, "snapshot"), onSnapshot)
}

func onSnapshot(c mqtt.Client, req core.Request) core.Response {
	cam, err := gocv.OpenVideoCapture(0)
	if err != nil {
		return core.NewResponse(fmt.Sprintf("Could not open the camera: %s", err.Error()), 500)
	}
	// give the camera time to adjust lightning
	time.Sleep(100 * time.Millisecond)

	img := gocv.NewMat()
	if success := cam.Read(&img); !success {
		cam.Close()
		return core.NewResponse("Could not read any frame from the camera.", 500)
	}

	data, _ := img.DataPtrUint8()
	img.Close()
	cam.Close()
	response := core.NewResponse(hex.EncodeToString(data), 200)
	return response.WithContentType("image/jpeg")
}
