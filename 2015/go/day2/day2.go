package main

import (
	"bytes"
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"os"
	"sort"
)

func main() {
	if len(os.Args) < 2 {
		log.Fatal("Input filename required")
	}

	f, err := ioutil.ReadFile(os.Args[1])
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println("Day 2:")

	b := bytes.NewBuffer(f)
	dims := make([]int, 3)
	paper := 0
	ribbon := 0
	for {
		_, err := fmt.Fscanf(b, "%dx%dx%d\n", &dims[0], &dims[1], &dims[2])
		if err != nil {
			if err != io.EOF {
				log.Fatal(err)
			}
			break
		}
		sort.Ints(dims)
		paper += 2*dims[0]*dims[1] + 2*dims[1]*dims[2] + 2*dims[2]*dims[0] + dims[0]*dims[1]
		ribbon += 2*dims[0] + 2*dims[1] + dims[0]*dims[1]*dims[2]
	}
	fmt.Printf("\tSolution 1: %d\n", paper)
	fmt.Printf("\tSolution 2: %d\n", ribbon)
}
