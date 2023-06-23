package main

import (
	"fmt"

	"github.com/dieklingel-core/server"
)

func main() {
	fmt.Println("Hello World!")
	server.Init(8081)
}
