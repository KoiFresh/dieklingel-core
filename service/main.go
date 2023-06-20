package main

import (
	"fmt"
	"net/http"
)

func main() {
	fmt.Println("Hello World!")

	var err error = http.ListenAndServe(":8081", nil)
	if err != nil {
		println("Cannot start the webserver! " + err.Error())
	}
}
