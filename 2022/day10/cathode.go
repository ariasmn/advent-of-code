package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
)

var queue = map[int]int{} // We save how much shall we add in each cycle
var x int = 1             // Value of X

func part1() {
	file, err := os.Open("input.txt")
	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)

	// We only parse input in this for loop
	cycle := 1

	for scanner.Scan() {
		line := scanner.Text()
		splittedLine := strings.Split(line, " ")

		if len(splittedLine) > 1 { // Each addx takes two turns to complete
			queue[cycle+2], _ = strconv.Atoi(splittedLine[1])
			cycle += 2
		} else { // If the instruction is noop, we just increase the cycle counter
			cycle += 1
		}
	}

	// Here we calculate everything
	cycle = 0
	sumStrength := 0

	for len(queue) > 0 {
		runQueue(cycle)
		if cycle == 20 || cycle == 60 || cycle == 100 || cycle == 140 || cycle == 180 || cycle == 220 {
			sumStrength += cycle * x
		}
		cycle++
	}

	fmt.Printf("Part 1 solution: %d\n", sumStrength)
}

func part2() {
	file, err := os.Open("input.txt")
	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)

	x = 1
	cycle := 1

	for scanner.Scan() {
		line := scanner.Text()
		splittedLine := strings.Split(line, " ")

		if len(splittedLine) > 1 {
			queue[cycle+2], _ = strconv.Atoi(splittedLine[1])
			cycle += 2
		} else {
			cycle += 1
		}
	}

	cycle = 0

	fmt.Println("Part 2 solution:")
	for len(queue) > 0 {
		// We use the cycle+1 since we have to run the instructions at the start and not at the end like in part 1
		// Harder to control the start/end manually than doing this
		runQueue(cycle + 1)
		if cycle%40 == 0 && cycle <= 220 {
			fmt.Println()
		}
		if x-1 == cycle%40 || x == cycle%40 || x+1 == cycle%40 {
			fmt.Print("#")
		} else {
			fmt.Print(" ")
		}
		cycle++
	}
}

func runQueue(cycle int) {
	x += queue[cycle]
	delete(queue, cycle) // Once we've done a task in the queue, we delete it
}

func main() {
	part1()
	part2()
}
