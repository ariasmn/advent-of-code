package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
)

var position int

// Both parts are basically the same, only the length changes, and could be refactored into a single function

func part1() {
	file, err := os.Open("input.txt")

	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)
	position = 0

	for scanner.Scan() {
		line := scanner.Text()

		for i := range line {
			if allDifferent(line[i : 4+i]) {
				position = i + 4
				break
			}
		}
	}

	fmt.Printf("Part 1 solution: %d\n", position)
}

func part2() {
	file, err := os.Open("input.txt")

	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)
	position = 0

	for scanner.Scan() {
		line := scanner.Text()

		for i := range line {
			if allDifferent(line[i : 14+i]) {
				position = i + 14
				break
			}
		}
	}

	fmt.Printf("Part 2 solution: %d\n", position)
}

func allDifferent(input string) bool {
	bytes := map[rune]bool{}

	for _, char := range input {
		bytes[char] = true
	}

	return len(bytes) == len(input) // Check if the length of the map is the same as the input
}

func main() {
	part1()
	part2()
}
