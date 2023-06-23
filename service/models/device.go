package models

type Device struct {
	Token string `json:"token" required:"true"`
}

func NewDevice(token string) *Device {
	return &Device{
		Token: token,
	}
}
