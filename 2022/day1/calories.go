//Part one

package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"strconv"
)

func main() {
	file, err := os.Open("calories_input.txt")

	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)

	//We could use a map/slice for this but not really worth it
	elfMaxCalories := 0
	elfMaxPosition := 0
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
			elfMaxPosition = currentElfPosition
		}
	}

	fmt.Printf("Elf number %d carrying %d calories\n", elfMaxPosition, elfMaxCalories)

	if err := scanner.Err(); err != nil {
		log.Fatal(err)
	}
}
