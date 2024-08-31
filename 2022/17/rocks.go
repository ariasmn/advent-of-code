package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
)

type CacheInput struct {
	step   int
	height int
}

type Coord struct {
	x int
	y int
}
type Rock []Coord

var stream string // The jet pattern stream

// The grid, which is 7 units wide. We invert the coords since it makes more sense later on.
// For each y coord, we have 7 x positions. This makes it easier to check, append... etc.
var grid [][7]bool

var directions = map[byte]int{
	'>': 1,
	'<': -1,
}

func part1() {
	var height int = -1 // We start at -1 since we adding the floor while spawning the rock, to avoid problems after 1st iteration
	var streamStep int = 0
	for step := 0; step < 2022; step++ {
		rock := newRock(step%5, height+1) // Modular arithmetic to get which rock has to drop now

		rock = horizontalMove(rock, streamStep) // Before we enter the fall/move loop, we need to move the rock horizontally
		streamStep++

		for canFall(rock) { // While the rock can fall, we keep moving it
			rock = verticalMove(rock)
			rock = horizontalMove(rock, streamStep)
			streamStep++
		}
		height = restRock(rock, height)
	}

	fmt.Printf("Part 1 solution: %d\n", height+1) // +1 since the program works without a floor, we add it here
}

// The idea on this is find repetition in the loop, and instead of simulating it, we calculate the height gained on the repetition
// Then, multiply n times, where n=number of repetitions left and sum current height
func part2() {
	grid = [][7]bool{}
	var steps int = 1_000_000_000_000
	var height int = -1
	var streamStep int = 0
	var cache = make(map[string]CacheInput)

	defer func() {
		fmt.Printf("Part 2 solution: %d\n", height)
	}()

	for step := 0; step < steps; step++ {
		rock := newRock(step%5, height+1)
		rock = horizontalMove(rock, streamStep)
		streamStep++

		for canFall(rock) {
			rock = verticalMove(rock)
			rock = horizontalMove(rock, streamStep)
			streamStep++
		}
		height = restRock(rock, height)

		// This is the for the cache map. It's formed by the rock that just rested and the last jet pattern finished (position on the input file)
		key := fmt.Sprintf("%d->%d", step%5, streamStep%len(stream))
		if cacheValue, ok := cache[key]; ok { // We check if already in the cache map
			// The timesLeft holds how many times we need to multiply the current height
			// In other words, how many times would've been needed to do the same thing to finish in a loop
			timesLeft := (steps - step) / (step - cacheValue.step)

			// This will tell us if there is a "hanging" step
			// So, if it's != 0, it means that we cannot calculate it because we would somehow need to add the hanging step's height (which we don't know)
			hanging := (steps - step) % (step - cacheValue.step)
			if hanging == 0 {
				height = height + (height-cacheValue.height)*timesLeft
				return
			}
		} else {
			cache[key] = CacheInput{step, height}
		}
	}

	fmt.Println()
}

// Props to: https://github.com/zbyju/advent-of-code/blob/1789e9b49f864e109a9932dac330fa93686dfc1d/2022/day17/part1.go#L16
func newRock(step, currentHeight int) Rock { // All the rocks start in X = 2 and Y = currentHeight + 3 or more if it's not the top part
	if step == 0 {
		return Rock{{2, currentHeight + 3}, {3, currentHeight + 3}, {4, currentHeight + 3}, {5, currentHeight + 3}}
	}
	if step == 1 {
		return Rock{{3, currentHeight + 3}, {2, currentHeight + 4}, {3, currentHeight + 4}, {4, currentHeight + 4}, {3, currentHeight + 5}}
	}
	if step == 2 {
		return Rock{{2, currentHeight + 3}, {3, currentHeight + 3}, {4, currentHeight + 3}, {4, currentHeight + 4}, {4, currentHeight + 5}}
	}
	if step == 3 {
		return Rock{{2, currentHeight + 3}, {2, currentHeight + 4}, {2, currentHeight + 5}, {2, currentHeight + 6}}
	}
	if step == 4 {
		return Rock{{2, currentHeight + 3}, {3, currentHeight + 3}, {2, currentHeight + 4}, {3, currentHeight + 4}}
	}

	panic("No rock to generate found")
}

func verticalMove(rock Rock) Rock { // Generate new rock from current rock
	newPosRock := Rock{}
	for _, part := range rock {
		newPosRock = append(newPosRock, Coord{part.x, part.y - 1}) // We just decrease the Y pos by 1 on each part of the rock
	}

	return newPosRock
}

func horizontalMove(rock Rock, step int) Rock {
	change := getJetMove(step) // We get if it should move left or right
	newPosRock := Rock{}
	for _, part := range rock {
		newPartCoord := Coord{part.x + change, part.y}
		if !checkEmptyCoord(newPartCoord) { // We check if rock's each part is possible to place on the new pos
			return rock // In case it's not posible, we return the initial rock before starting to move
		}
		newPosRock = append(newPosRock, newPartCoord)
	}

	return newPosRock
}

func getJetMove(step int) int {
	streamLength := len(stream)
	symbol := stream[step%streamLength] // Modular arithmetic again

	return directions[symbol]
}

func canFall(rock Rock) bool { // This checks if the rock can fall or have to rest in current pos
	if rock[0].y < 0 { // If current rock y pos is less than 0, have to rest. This should never haven happen
		return false
	}
	for _, part := range rock { // We get each rock coords and check if will collide
		if !checkEmptyCoord(Coord{part.x, part.y - 1}) { // -1 since we want the part to fall
			return false
		}
	}
	return true
}

func restRock(rock Rock, currentHeight int) int { // When a rock can't move anymore, we register it and update the height
	for _, part := range rock {
		if part.y > currentHeight {
			currentHeight = part.y
		}
		for part.y >= len(grid) { // If piece's height greater than grid height, we extend the grid
			grid = append(grid, [7]bool{false, false, false, false, false, false, false})
		}
		grid[part.y][part.x] = true
	}

	return currentHeight
}

func checkEmptyCoord(coord Coord) bool { // This checks if a single coord is empty or not
	if coord.x >= 7 || coord.x < 0 || coord.y < 0 { // Check grid limits
		return false
	}
	if coord.y >= len(grid) { // Check the height of the grid. If the current coord is bigger, we return true since it's not occupied
		return true
	}

	return !grid[coord.y][coord.x] // Return if the coord is already filled
}

func parseJet() {
	file, err := os.Open("input.txt")
	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)
	scanner.Scan()
	stream = scanner.Text()
}

func main() {
	parseJet()
	part1()
	part2()
}
