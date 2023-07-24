package handlers

import (
	"encoding/json"
	"fmt"
	"log"
	"path"
	"strings"

	"github.com/dieklingel-core/core"
	mqtt "github.com/eclipse/paho.mqtt.golang"
	"github.com/pion/mediadevices"
	"github.com/pion/mediadevices/pkg/codec/opus"
	"github.com/pion/mediadevices/pkg/codec/vpx"
	_ "github.com/pion/mediadevices/pkg/driver/camera"
	_ "github.com/pion/mediadevices/pkg/driver/microphone"
	"github.com/pion/mediadevices/pkg/prop"
	"github.com/pion/webrtc/v3"
)

var connections map[string]*webrtc.PeerConnection = make(map[string]*webrtc.PeerConnection)

func RegisterRtcHandler(prefix string, client mqtt.Client) {
	register(client, path.Join(prefix, "connections"), onGetConnections)
	register(client, path.Join(prefix, "connections", "create", "+"), onCreateConnection)
	register(client, path.Join(prefix, "connections", "close", "+"), onCloseConnection)
	register(client, path.Join(prefix, "connections", "candidate", "+"), onAddCandidate)
}

func onGetConnections(c mqtt.Client, req core.Request) core.Response {
	json, err := json.Marshal(connections)
	if err != nil {
		return core.NewResponse(fmt.Sprintf("Could not encode: %s.", err), 500)
	}

	return core.NewResponse(string(json), 200)
}

var stream mediadevices.MediaStream

func onCreateConnection(client mqtt.Client, req core.Request) core.Response {
	pathSegments := strings.Split(req.RequestPath, "/")
	id := pathSegments[len(pathSegments)-1]

	if _, exists := connections[id]; exists {
		return core.NewResponse(fmt.Sprintf("Cannot create a connection with id '%s' because a connection with this id already exists.", id), 409)
	}

	vpxParams, err := vpx.NewVP8Params()
	if err != nil {
		panic(err)
	}
	vpxParams.BitRate = 500_000 // 500kbps

	opusParams, err := opus.NewParams()
	if err != nil {
		panic(err)
	}
	codecSelector := mediadevices.NewCodecSelector(
		mediadevices.WithVideoEncoders(&vpxParams),
		mediadevices.WithAudioEncoders(&opusParams),
	)

	stream, _ = mediadevices.GetUserMedia(mediadevices.MediaStreamConstraints{
		Video: func(constraint *mediadevices.MediaTrackConstraints) {
			// Query for ideal resolutions
			constraint.Width = prop.Int(600)
			constraint.Height = prop.Int(400)

		},
		Audio: func(mtc *mediadevices.MediaTrackConstraints) {},
		Codec: codecSelector,
	})

	mediaEngine := webrtc.MediaEngine{}
	codecSelector.Populate(&mediaEngine)
	api := webrtc.NewAPI(webrtc.WithMediaEngine(&mediaEngine))

	peerConnection, err := api.NewPeerConnection(
		webrtc.Configuration{
			ICEServers: []webrtc.ICEServer{
				{
					URLs: []string{"stun:stun.l.google.com:19302"},
				},
			},
			SDPSemantics: webrtc.SDPSemanticsUnifiedPlan,
		},
	)
	if err != nil {
		peerConnection.Close()
		return core.NewResponse(fmt.Sprintf("Cannot create a connection: %s", err.Error()), 500)
	}

	for _, track := range stream.GetTracks() {
		if _, err := peerConnection.AddTrack(track); err != nil {
			log.Printf("cannot add track to connection: %s\r\n", err.Error())
		}
	}

	peerConnection.OnICECandidate(func(c *webrtc.ICECandidate) {
		if c == nil {
			log.Println("OnICECandidate Callback was called with nil reference and will be silently ignored.")
			return // dont know why the callback is called with nil, but it is the case
		}
		candidate, _ := json.Marshal(c.ToJSON())
		request := core.NewSocketRequest(string(candidate))
		request.Method = "CONNECT"
		json, _ := json.Marshal(request)

		client.Publish(path.Join(id, "connection", "candidate"), 2, false, string(json))
	})

	peerConnection.OnTrack(func(track *webrtc.TrackRemote, r *webrtc.RTPReceiver) {
		fmt.Printf("Track has started, of type %d: %s \n", track.PayloadType(), track.Codec().MimeType)
		// TODO: play stream, if audio
	})

	var offer *webrtc.SessionDescription = &webrtc.SessionDescription{}
	if err := json.Unmarshal([]byte(req.Body), offer); err != nil {
		panic(err.Error())
	}
	if err := peerConnection.SetRemoteDescription(*offer); err != nil {
		panic(err.Error())
	}

	answer, err := peerConnection.CreateAnswer(&webrtc.AnswerOptions{})
	peerConnection.SetLocalDescription(answer)
	if err != nil {
		panic(err.Error())
	}

	json, err := json.Marshal(answer)
	if err != nil {
		panic(err.Error())
	}

	connections[id] = peerConnection

	return core.NewResponse(string(json), 201)
}

func onCloseConnection(client mqtt.Client, req core.Request) core.Response {
	pathSegments := strings.Split(req.RequestPath, "/")
	id := pathSegments[len(pathSegments)-1]

	for _, track := range stream.GetTracks() {
		defer track.Close()
	}

	if connection, exists := connections[id]; exists {
		connection.Close()
	}

	return core.NewResponse("", 200)
}

func onAddCandidate(client mqtt.Client, req core.Request) core.Response {
	pathSegments := strings.Split(req.RequestPath, "/")
	id := pathSegments[len(pathSegments)-1]

	candidate := &webrtc.ICECandidateInit{}
	if err := json.Unmarshal([]byte(req.Body), candidate); err != nil {
		log.Printf("could not parse candidate: %s", err.Error())
		return core.NewResponse(fmt.Sprintf("the canidate could not be parsed: %s", err.Error()), 400)
	}

	if connection, exists := connections[id]; exists {
		if err := connection.AddICECandidate(*candidate); err != nil {
			log.Printf("could not add candidate: %s", err.Error())
			return core.NewResponse(fmt.Sprintf("could not add the candidate: %s", err.Error()), 500)
		}
		return core.NewResponse("ok", 200)
	}

	return core.NewResponse("the requested resource does not exist", 404)
}
