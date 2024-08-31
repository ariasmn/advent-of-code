package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"sort"
	"strconv"
)

func main() {
	part1()
	part2()
}

func part1() {
	file, err := os.Open("input.txt")

	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)

	//We could use a map/slice for this but not really worth it
	elfMaxCalories := 0
	currentElfPosition := 1
	currentElfCalories := 0

	for scanner.Scan() {
		if scanner.Text() == "" {
			currentElfCalories = 0
			currentElfPosition++
			continue
		}

		line, err := strconv.Atoi(scanner.Text())

		if err != nil {
			log.Fatal(err)
		}

		currentElfCalories += line

		if currentElfCalories > elfMaxCalories {
			elfMaxCalories = currentElfCalories
		}
	}

	fmt.Printf("Part 1 result: %d\n", elfMaxCalories)

	if err := scanner.Err(); err != nil {
		log.Fatal(err)
	}
}

func part2() {
	file, err := os.Open("input.txt")

	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)

	//Using slices since we don't need the elf position
	var elfCalories []int
	currentElfCalories := 0

	for scanner.Scan() {
		if scanner.Text() == "" {
			elfCalories = append(elfCalories, currentElfCalories)
			currentElfCalories = 0
			continue
		}

		line, err := strconv.Atoi(scanner.Text())

		if err != nil {
			log.Fatal(err)
		}

		currentElfCalories += line
	}

	//Sort the slice
	sort.Sort(sort.Reverse(sort.IntSlice(elfCalories)))

	//Sum the first three
	totalThreeCalories := 0
	for _, current := range elfCalories[0:3] {
		totalThreeCalories += current
	}

	fmt.Printf("Part 2 result: %d\n", totalThreeCalories)

	if err := scanner.Err(); err != nil {
		log.Fatal(err)
	}
}
