package main

import (
	"fmt"
	"io/ioutil"
	"log"
	"os"
)

func main() {
	if len(os.Args) < 2 {
		log.Fatal("Input filename required")
	}

	f, err := ioutil.ReadFile(os.Args[1])
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println("Day 1:")

	floor := 0
	position := 0
	for i, c := range f {
		i++
		switch c {
		case '(':
			floor++
		case ')':
			floor--
		}
		if floor == -1 && position < 1 {
			position = i
		}
	}

	fmt.Printf("\tSolution 1: %d\n", floor)
	fmt.Printf("\tSolution 2: %d\n", position)
}
