package main

import (
	"fmt"
	"io/ioutil"
	"log"
	"os"
)

func main() {
	if len(os.Args) < 2 {
		log.Fatal("Must provide input filename")
	}

	input, err := ioutil.ReadFile(os.Args[1])
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println("Day 1")

	floor := 0
	position := 0

	for i, c := range input {
		switch c {
		case '(':
			floor++
		case ')':
			floor--
		}
		if position < 1 && floor == -1 {
			position = i + 1
		}
	}

	fmt.Printf("\tSolution 1: %d\n", floor)
	fmt.Printf("\tSolution 2: %d\n", position)
}
