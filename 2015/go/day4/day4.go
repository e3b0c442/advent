package main

import (
	"bytes"
	"crypto/md5"
	"fmt"
	"io/ioutil"
	"log"
	"os"
	"strconv"
	"strings"
)

func main() {
	if len(os.Args) < 2 {
		log.Fatal("Input filename required")
	}

	f, err := ioutil.ReadFile(os.Args[1])
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println("Day 4:")

	i := 1
	for {
		sb := &strings.Builder{}
		sb.Write(f)
		sb.WriteString(strconv.Itoa(i))
		sum := md5.Sum([]byte(sb.String()))
		if bytes.Equal(sum[0:2], []byte{0, 0}) && sum[2] < 0x10 {
			break
		}
		i++
	}

	fmt.Printf("\tSolution 1: %d\n", i)

	for {
		sb := &strings.Builder{}
		sb.Write(f)
		sb.WriteString(strconv.Itoa(i))
		sum := md5.Sum([]byte(sb.String()))
		if bytes.Equal(sum[0:3], []byte{0, 0, 0}) {
			break
		}
		i++
	}
	fmt.Printf("\tSolution 2: %d\n", i)

}
