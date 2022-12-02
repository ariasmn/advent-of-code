package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"strings"
)

// Taking more of a "competitive programming" approach on this one. You WON'T find clean code here.

var opponent = map[string]int{
	"A": 1,
	"B": 2,
	"C": 3,
}

var player = map[string]int{
	"X": 1,
	"Y": 2,
	"Z": 3,
}

// Having two "lookup" maps is faster than iterating over one.
var beatersLosers = map[string]string{
	"A": "C",
	"B": "A",
	"C": "B",
}

var losersBeaters = map[string]string{
	"A": "B",
	"B": "C",
	"C": "A",
}

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

	totalScore := 0

	for scanner.Scan() {
		line := scanner.Text()
		match := strings.Split(line, " ")

		totalScore += matchResultPoints(match)
		totalScore += player[match[1]]
	}

	fmt.Printf("Part 1 result: %d\n", totalScore)
}

func part2() {
	file, err := os.Open("input.txt")

	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)

	totalScore := 0

	for scanner.Scan() {
		line := scanner.Text()
		match := strings.Split(line, " ")

		wantedResult := match[1]

		/**
		* "X" => Lose
		* "Y" => Draw
		* "Z" => Win
		**/
		if wantedResult == "X" {
			whatToPlay := beatersLosers[match[0]]
			totalScore += opponent[whatToPlay]
		}

		if wantedResult == "Y" {
			totalScore += 3
			totalScore += opponent[match[0]]
		}

		if wantedResult == "Z" {
			whatToPlay := losersBeaters[match[0]]
			totalScore += 6
			totalScore += opponent[whatToPlay]
		}
	}

	fmt.Printf("Part 2 result: %d\n", totalScore)

}

func matchResultPoints(match []string) int {
	opponentMove := match[0]
	playerMove := match[1]

	playerValue := player[playerMove]
	opponentValue := opponent[opponentMove]

	matchResult := playerValue - opponentValue

	/**
	* If equal, is a draw
	* If consecutive (1, -1), the bigger wins
	* If no consecutive, the smaller wins
	**/
	if matchResult == 0 {
		return 3
	}

	//Could create and int abs function but since it is simple we do it like that
	if matchResult == 1 || matchResult == -1 {
		if playerValue == max(playerValue, opponentValue) {
			return 6
		}
	} else {
		if playerValue == min(playerValue, opponentValue) {
			return 6
		}
	}

	return 0
}

func min(a, b int) int {
	if a < b {
		return a
	}
	return b
}

func max(a, b int) int {
	if a > b {
		return a
	}
	return b
}
