package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
)

type Coord struct {
	x int
	y int
}

func (coord Coord) add(anotherCoord Coord) Coord { // Add coord to a current coord
	return Coord{coord.x + anotherCoord.x, coord.y + anotherCoord.y}
}

type Blizzard struct { // This will hold each blizzard current position, dir and where should it go when wrapping around
	current    Coord
	direction  rune
	wrapAround Coord
}

var directions = map[rune]Coord{ // How the blizzard should move given the direction
	'^': {0, -1},
	'v': {0, 1},
	'>': {1, 0},
	'<': {-1, 0},
}

var sizeX int            // Basin's x size
var sizeY int            // Basin's y size
var blizzards []Blizzard // All the blizzards will be store there

func part1() {
	parseBasin()
	calculateBlizzardsWrapAround()

	var starting = Coord{0, -1}          // This is our starting point. As said, the y coord is -1 since we start counting the basin only where blizzards can go
	var target = Coord{sizeX - 1, sizeY} // We we want to end

	minutes := crossBasin(starting, target)

	fmt.Printf("Part 1 solution: %d\n", minutes)
}

func part2() {
	parseBasin()
	calculateBlizzardsWrapAround()

	var starting = Coord{0, -1}
	var target = Coord{sizeX - 1, sizeY}

	firstTravel := crossBasin(starting, target)
	secondTravel := crossBasin(target, starting)
	thirdTravel := crossBasin(starting, target)

	fmt.Printf("Part 2 solution: %d\n", firstTravel+secondTravel+thirdTravel)
}

// Could do this using BFS, and the idea behind this is quite similar
func crossBasin(starting, target Coord) int {
	minutes := 0
	currentPerson := map[Coord]bool{} // This will hold the safe coords
	currentPerson[starting] = true

	for !currentPerson[target] { // We keep looping until our target is in the safe coords
		// First, we move our blizzards
		updatedBlizzards := map[Coord]bool{}
		for index, blizzard := range blizzards {
			newBlizzard := blizzard.current.add(directions[blizzard.direction]) // We calculate the new blizzard coord
			if checkBounds(newBlizzard) {
				blizzards[index].current = newBlizzard // We update the blizzard within the general slice
			} else {
				blizzards[index].current = blizzard.wrapAround // If out of bounds, the new position is the wrap around one
			}
			updatedBlizzards[blizzards[index].current] = true // We add it to the updated blizzards map
		}

		// Now, we find the possible coords to visit
		possiblePerson := map[Coord]bool{}
		for coord := range currentPerson {
			if !(updatedBlizzards[coord]) { // If no blizzard, this is a safe spot
				possiblePerson[coord] = true
			}
			for _, neighbor := range directions { // We check possible neighbors in this current position
				possibleNeighbor := coord.add(neighbor)
				if possibleNeighbor == target {
					possiblePerson[possibleNeighbor] = true // We add the if it's the target coord
					break
				}
				if checkBounds(possibleNeighbor) && !updatedBlizzards[possibleNeighbor] { // If there are no blizzards and inside the basin, we add id to the possible steps
					possiblePerson[possibleNeighbor] = true
				}
			}
		}
		currentPerson = possiblePerson // The new map to start with is the one generated in this iteration
		minutes++
	}

	return minutes
}

func checkBounds(coord Coord) bool {
	return (coord.x >= 0 && coord.x < sizeX && coord.y >= 0 && coord.y < sizeY) // We check if it's inside the basin
}

func parseBasin() {
	blizzards = nil // Clear the blizzards slice, just in case it was already filled

	file, err := os.Open("input.txt")
	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)
	var row int = 0
	for scanner.Scan() {
		line := scanner.Text()
		if row == 0 { // Only do this on the first line. The result wouldn't change, but to avoid repetition
			sizeX = len(line) - 2 // "-2", which are both side walls
			row++
			continue // We don't want to parse anything else in this line
		}
		for index, val := range line {
			if val == '^' || val == 'v' || val == '>' || val == '<' { // Ignore everything unless we have to parse a blizzard
				parsedBlizzard := Blizzard{Coord{index - 1, row - 1}, val, Coord{}} // Set the wrap around coord as empty, since we can't calculate it until we have the sizeY
				blizzards = append(blizzards, parsedBlizzard)
			}
		}
		row++ // Increase the row, which keep track of the y coord
	}
	sizeY = row - 2 // "-2" also on the Y coord, since the start and end line can't be visited by a blizzard
}

func calculateBlizzardsWrapAround() {
	// All parsed, now we need to calculate the wrap around coord for each blizzard
	for index, blizzard := range blizzards {
		if blizzard.direction == '^' {
			blizzards[index].wrapAround = Coord{blizzard.current.x, sizeY - 1}
		}
		if blizzard.direction == 'v' {
			blizzards[index].wrapAround = Coord{blizzard.current.x, 0}
		}
		if blizzard.direction == '>' {
			blizzards[index].wrapAround = Coord{0, blizzard.current.y}
		}
		if blizzard.direction == '<' {
			blizzards[index].wrapAround = Coord{sizeX - 1, blizzard.current.y}
		}
	}
}

func main() {
	// We parse the basin and calculate the wrap arounds in each part so the blizzards are not already modified
	part1()
	part2()
}
