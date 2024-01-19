package main

import (
	"fmt"
	"os"
)

func main() {
	args := os.Args

	if len(args) != 2 {
		fmt.Println("Usage: generate_ast <output directory>")
		os.Exit(1)
	}
}
