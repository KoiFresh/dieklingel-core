package core

type Response struct {
	Body  string `json:"body"`
	empty bool
}

func NewEmptyResponse() Response {
	return Response{
		empty: true,
	}
}

func NewResponse(body string) Response {
	return Response{
		Body: body,
	}
}

func (res *Response) IsEmpty() bool {
	return res.empty
}
