package models

type Action struct {
	Regex string
	Lane  string
}

type ActionResult struct {
	Result string
}

func NewAction(regex string, lane string) *Action {
	return &Action{
		Regex: regex,
		Lane:  lane,
	}
}

func (action *Action) Run() (*ActionResult, error) {
	return nil, nil
}
