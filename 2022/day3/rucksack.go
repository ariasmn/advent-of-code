package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
)

func part1() {
	file, err := os.Open("input.txt")

	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)
	prioritySum := 0

	for scanner.Scan() {
		// we are storing the ASCII values instead of creating a map
		compartment := [150]bool{}
		line := scanner.Text()
		splitLength := len(line) / 2

		for _, char := range line[:splitLength] {
			compartment[char] = true
		}

		for _, char := range line[splitLength:] {
			if compartment[char] {
				prioritySum += calcPriority(int(char))
				// It's not listed that only 1 item type can be shared between both compartments, but I failed with the first try without it so...
				break
			}
		}
	}

	fmt.Printf("Part 1 result: %d\n", prioritySum)
}

func part2() {
	file, err := os.Open("input.txt")

	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)
	prioritySum := 0

	for scanner.Scan() {
		compartments := [3][150]bool{}
		for i := 0; i < 3; i++ {
			line := scanner.Text()
			for _, char := range line {
				compartments[i][char] = true
			}
			// Don't advance the scanner if the loop is ending
			if i != 2 {
				scanner.Scan()
			}
		}
		// First char to check in ASCII is A (65), last one is z(122)
		// We check if each char is inside all compartments
		for char := 65; char < 123; char++ {
			if compartments[0][char] && compartments[1][char] && compartments[2][char] {
				prioritySum += calcPriority(char)
				break
			}
		}
	}

	fmt.Printf("Part 2 result: %d\n", prioritySum)
}

func calcPriority(char int) int {
	// We use the ASCII values to calculate the priority
	// If lowercase, difference is a - 1
	if 'a' <= char && 'z' >= char {
		return char - ('a' - 1)
	}
	// If uppercase, difference is A - 27
	return char - ('A' - 27)
}

func main() {
	part1()
	part2()
}
