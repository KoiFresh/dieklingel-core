package models

type Sign struct {
	Identifier   string
	InterfaceURL string
	AudioURL     string
}

func NewSign(identifier string, identifierURL string, audioURL string) *Sign {
	return &Sign{
		Identifier:   identifier,
		InterfaceURL: identifierURL,
		AudioURL:     audioURL,
	}
}
