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
	fmt.Println("Day 3:")

	var x, y, minX, maxX, minY, maxY, minX2, minY2, maxX2, maxY2 int
	x2 := make([]int, 2)
	y2 := make([]int, 2)

	s := 0
	for _, c := range f {
		switch c {
		case '<':
			x--
			x2[s]--
			if x < minX {
				minX = x
			}
			if x2[s] < minX2 {
				minX2 = x2[s]
			}
		case 'v':
			y--
			y2[s]--
			if y < minY {
				minY = y
			}
			if y2[s] < minY2 {
				minY2 = y2[s]
			}
		case '>':
			x++
			x2[s]++
			if x > maxX {
				maxX = x
			}
			if x2[s] > maxX2 {
				maxX2 = x2[s]
			}
		case '^':
			y++
			y2[s]++
			if y > maxY {
				maxY = y
			}
			if y2[s] > maxY2 {
				maxY2 = y2[s]
			}
		}
		s ^= 1
	}

	w := maxX - minX + 1
	h := maxY - minY + 1
	w2 := maxX2 - minX2 + 1
	h2 := maxY2 - minY2 + 1

	grid := make([][]bool, w)
	for i := range grid {
		grid[i] = make([]bool, h)
	}
	grid2 := make([][]bool, w2)
	for i := range grid2 {
		grid2[i] = make([]bool, h2)
	}

	x = -minX
	y = -minY
	x2[0] = -minX2
	x2[1] = -minX2
	y2[0] = -minY2
	y2[1] = -minY2
	s = 0

	grid[x][y] = true
	grid2[x2[s]][y2[s]] = true
	for _, c := range f {
		switch c {
		case '<':
			x--
			x2[s]--
		case 'v':
			y--
			y2[s]--
		case '>':
			x++
			x2[s]++
		case '^':
			y++
			y2[s]++
		}
		grid[x][y] = true
		grid2[x2[s]][y2[s]] = true
		s ^= 1
	}

	count := 0
	for _, r := range grid {
		for _, c := range r {
			if c {
				count++
			}
		}
	}

	count2 := 0
	for _, r := range grid2 {
		for _, c := range r {
			if c {
				count2++
			}
		}
	}

	fmt.Printf("\tSolution 1: %d\n", count)
	fmt.Printf("\tSolution 2: %d\n", count2)

}
