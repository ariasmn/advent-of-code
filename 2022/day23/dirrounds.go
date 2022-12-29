package main

import (
	"bufio"
	"fmt"
	"log"
	"math"
	"os"
)

type Coord struct {
	x int
	y int
}

func (coord Coord) add(coordToAdd Coord) Coord { // Add coord to current coord
	return Coord{coord.x + coordToAdd.x, coord.y + coordToAdd.y}
}

type Elf struct {
	current Coord
	next    Coord
}

// All the possible neighbors we should check
var north = Coord{0, -1}
var northeast = Coord{1, -1}
var northwest = Coord{-1, -1}
var south = Coord{0, 1}
var southeast = Coord{1, 1}
var southwest = Coord{-1, 1}
var west = Coord{-1, 0}
var east = Coord{1, 0}

var occupied = map[Coord]bool{} // Elves' positions
var elves = []*Elf{}            // Slice pointing to each elf
var directions = map[int]byte{  // Possible directions that each elf may propose
	0: 'N',
	1: 'S',
	2: 'W',
	3: 'E',
}

func part1() {
	parseInput()
	for round := 0; round < 10; round++ {
		proposed := map[Coord]bool{} // Map of each round's proposed moves
		repeated := map[Coord]bool{} // Map that will hold those destinations proposed by more than one elf
		for _, elf := range elves {
			proposedMove := proposeMove(round, *elf)
			if _, found := proposed[proposedMove]; found {
				repeated[proposedMove] = true
			}
			proposed[proposedMove] = true
			elf.next = proposedMove
		}
		occupied = endRound(repeated)
	}

	fmt.Printf("Part 1 solution: %d\n", countEmptyTiles())
}

// We do the same logic as in part1, we change the endRound function behaviour
func part2() {
	parseInput()
	var notMovingRound int

	defer func() {
		fmt.Printf("Part 2 solution: %d\n", notMovingRound)
	}()

	for round := 0; round > -1; round++ {
		proposed := map[Coord]bool{}
		repeated := map[Coord]bool{}
		notMoveCount := 0
		for _, elf := range elves {
			proposedMove := proposeMove(round, *elf)
			if _, found := proposed[proposedMove]; found {
				repeated[proposedMove] = true
			}
			proposed[proposedMove] = true
			elf.next = proposedMove
		}
		occupied, notMoveCount = endRoundPartTwo(repeated) // We also get how many elves didn't move at the end of the round
		if notMoveCount == len(elves) {
			notMovingRound = round + 1
			return
		}
	}
}

func countEmptyTiles() int {
	var minX, minY = math.MaxInt, math.MaxInt
	var maxX, maxY = math.MinInt, math.MinInt

	// We find the max and min coord values
	for _, elf := range elves {
		minX = intMin(minX, elf.current.x)
		maxX = intMax(maxX, elf.current.x)
		minY = intMin(minY, elf.current.y)
		maxY = intMax(maxY, elf.current.y)
	}

	// We align the edges using the max/min coords. Then, calculate the number of total coords and substract the number of elves.
	return (maxX-minX+1)*(maxY-minY+1) - len(elves)
}

func endRound(repeated map[Coord]bool) map[Coord]bool {
	newOccupied := map[Coord]bool{}
	for _, elf := range elves {
		if _, found := repeated[elf.next]; found {
			elf.next = elf.current
		}
		elf.current = elf.next
		elf.next = Coord{}
		newOccupied[elf.current] = true
	}

	return newOccupied
}

// We could use the same function, but doing it like so to really see the differences
// The only thing that we do is keep a record of how many elves moved in the round and return it
func endRoundPartTwo(repeated map[Coord]bool) (map[Coord]bool, int) {
	newOccupied := map[Coord]bool{}
	var countNotMoving int
	for _, elf := range elves {
		if _, found := repeated[elf.next]; found {
			elf.next = elf.current
		}
		if elf.current == elf.next {
			countNotMoving++
		}
		elf.current = elf.next
		elf.next = Coord{}
		newOccupied[elf.current] = true
	}

	return newOccupied, countNotMoving
}

func proposeMove(currentRound int, elf Elf) Coord {
	northNeighbors := checkNorth(elf.current)
	southNeighbors := checkSouth(elf.current)
	westNeighbors := checkWest(elf.current)
	eastNeighbors := checkEast(elf.current)

	if northNeighbors && southNeighbors && westNeighbors && eastNeighbors { // If no neighbors, we do nothing
		return elf.current
	}

	// We loop for each possible direction, starting first with the preferred one, based on the round
	for dir := currentRound; dir < currentRound+4; dir++ {
		nextDir := directions[dir%4]
		if nextDir == 'N' && northNeighbors {
			return elf.current.add(north)
		}
		if nextDir == 'S' && southNeighbors {
			return elf.current.add(south)
		}
		if nextDir == 'W' && westNeighbors {
			return elf.current.add(west)
		}
		if nextDir == 'E' && eastNeighbors {
			return elf.current.add(east)
		}
	}

	return elf.current // If can't move, stay on the same position
}

// Should be a cleaner way to do those checker functions but it works and it's clear enough
func checkNorth(coord Coord) bool {
	adjN := coord.add(north)
	adjNE := coord.add(northeast)
	adjNW := coord.add(northwest)

	if occupied[adjN] || occupied[adjNE] || occupied[adjNW] { // An elf already in that coord
		return false
	}

	return true
}

func checkSouth(coord Coord) bool {
	adjS := coord.add(south)
	adjSE := coord.add(southeast)
	adjSW := coord.add(southwest)

	if occupied[adjS] || occupied[adjSE] || occupied[adjSW] {
		return false
	}

	return true
}

func checkWest(coord Coord) bool {
	adjW := coord.add(west)
	adjNW := coord.add(northwest)
	adjSW := coord.add(southwest)

	if occupied[adjW] || occupied[adjNW] || occupied[adjSW] {
		return false
	}

	return true
}

func checkEast(coord Coord) bool {
	adjE := coord.add(east)
	adjNE := coord.add(northeast)
	adjSE := coord.add(southeast)

	if occupied[adjE] || occupied[adjNE] || occupied[adjSE] {
		return false
	}

	return true
}

func parseInput() {
	occupied = map[Coord]bool{}
	elves = nil

	file, err := os.Open("input.txt")
	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)
	var row int // Row counter
	for scanner.Scan() {
		line := scanner.Text()
		for column, tile := range line { // Each char is a "column" for the coord
			if tile == '#' {
				currentCoord := Coord{column, row}
				elf := Elf{currentCoord, Coord{}}

				elves = append(elves, &elf)
				occupied[currentCoord] = true
			}
		}
		row++
	}
}

func intMin(a, b int) int {
	if a < b {
		return a
	}

	return b
}

func intMax(a, b int) int {
	if a > b {
		return a
	}

	return b
}

func main() {
	part1()
	part2()
}
