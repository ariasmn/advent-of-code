package main

import (
	"bufio"
	"fmt"
	"log"
	"math"
	"os"
	"strconv"
	"strings"
)

var headX, headY int
var tailX, tailY int

var knots [10][2]int // Slice that holds 10 arrays of [2]ints (x and y).

func part1() {
	file, err := os.Open("input.txt")
	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	positions := map[string]bool{} // Store the tail's positions as formatted strings

	scanner := bufio.NewScanner(file)

	for scanner.Scan() {
		line := scanner.Text()
		splittedLine := strings.Split(line, " ")

		direction := splittedLine[0]
		howMany, _ := strconv.ParseInt(splittedLine[1], 10, 0)

		positions[coordsAsString(tailX, tailY)] = true

		for howMany > 0 {
			if direction == "R" {
				moveRope(1, 0)
			}

			if direction == "L" {
				moveRope(-1, 0)
			}

			if direction == "U" {
				moveRope(0, 1)
			}

			if direction == "D" {
				moveRope(0, -1)
			}

			positions[coordsAsString(tailX, tailY)] = true
			howMany--
		}
	}

	fmt.Printf("Part 1 solution: %d\n", len(positions))
}

func part2() {
	file, err := os.Open("input.txt")
	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	positions := map[string]bool{} // Store the tail's positions as formatted strings

	scanner := bufio.NewScanner(file)

	for scanner.Scan() {
		line := scanner.Text()
		splittedLine := strings.Split(line, " ")

		direction := splittedLine[0]
		howMany, _ := strconv.ParseInt(splittedLine[1], 10, 0)

		for howMany > 0 {
			if direction == "R" {
				moveRopePart2(1, 0)
			}

			if direction == "L" {
				moveRopePart2(-1, 0)
			}

			if direction == "U" {
				moveRopePart2(0, 1)
			}

			if direction == "D" {
				moveRopePart2(0, -1)
			}

			positions[coordsAsString(knots[9][0], knots[9][1])] = true
			howMany--
		}
	}

	fmt.Printf("Part 2 solution: %d\n", len(positions))
}

func moveRope(directionX, directionY int) {
	headX += directionX
	headY += directionY

	if !isTailTouchingHead(headX, headY, tailX, tailY) {
		moveX := 0
		moveY := 0
		// We will always move if the diff it's a value other than 0
		// At max, we will always move 1 pos in each axis, however, we need to also handle negative values (left or down)
		// We could do two ifs (< 0 and > 0) for each axis, but maths can solve this better!
		xDiff := headX - tailX
		yDiff := headY - tailY

		// We divide the diff by the abs value of itself, to keep the sign
		if xDiff != 0 {
			moveX = xDiff / int(math.Abs(float64(xDiff)))
		}

		if yDiff != 0 {
			moveY = yDiff / int(math.Abs(float64(yDiff)))
		}

		tailX += moveX
		tailY += moveY
	}
}

func moveRopePart2(directionX, directionY int) { // Could've refactor to use a single func for both parts but already late
	for i := 1; i < 10; i++ {
		head := &knots[i-1]
		tail := &knots[i]

		if i == 1 { // Rope's head, will always move
			head[0] += directionX
			head[1] += directionY
		}

		if !isTailTouchingHead(head[0], head[1], tail[0], tail[1]) {
			moveX := 0
			moveY := 0
			xDiff := head[0] - tail[0]
			yDiff := head[1] - tail[1]

			if xDiff != 0 {
				moveX = xDiff / int(math.Abs(float64(xDiff)))
			}

			if yDiff != 0 {
				moveY = yDiff / int(math.Abs(float64(yDiff)))
			}

			tail[0] += moveX
			tail[1] += moveY
		}
	}
}

func isTailTouchingHead(hX, hY, tX, tY int) bool {
	// Calculate the differente between head and tail in both axis
	// If in both cases is greater than 1 (should be 2 max), the head and tail are not touching

	xDiff := hX - tX
	yDiff := hY - tY

	return math.Abs(float64(xDiff)) < 2.0 && math.Abs(float64(yDiff)) < 2.0
}

func coordsAsString(x, y int) string {
	return fmt.Sprintf("%d - %d", x, y)
}

func main() {
	part1()
	part2()
}
