//Part two

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
	file, err := os.Open("calories_input.txt")

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

	fmt.Printf("The three elves are carrying %d calories\n", totalThreeCalories)

	if err := scanner.Err(); err != nil {
		log.Fatal(err)
	}
}
