package main

import (
	"bufio"
	"errors"
	"fmt"
	"log"
	"os"
	"strconv"
)

// Doing this problem on a "Pythonic" way

var grid [][]int // The map that we are going to move in
var moves string // The moves

var tiles = map[byte]int{ // Map that holds the different possibilities we can find while parsing the grid
	' ': -1, // Empty, so it means that we need to wrap around
	'.': 1,  // Solid, meaning we can go there
	'#': 0,  // Wall, we can't go past
}

var dirs = map[byte]int{
	'R': 0,
	'D': 1,
	'L': 2,
	'U': 3,
}

func part1() {
	facing := dirs['R']
	col := 0
	row := 0
	for index := 0; index < len(moves); index++ { // We start moving
		move := moves[index]
		// Although we have 4 possible dirs, we can only find these two in the inputs
		if move == 'R' {
			facing = (facing + 1) % 4 // Is positive, so mod is enough
			continue
		}
		if move == 'L' {
			// It we turn left, some time we might find some trouble when it starts to set negative values, so we need to restart the value
			facing--        // We turn left
			if facing < 0 { // If less than 0, it means, that from facing Right we need to move to facing Up
				facing = dirs['U']
			}
			continue
		}
		indexStart := index                                                    // We will use this to parse 2 digits numbers
		for index < len(moves) && moves[index] != 'L' && moves[index] != 'R' { // We advance the index until we found a dir change
			index++
		}
		steps, err := strconv.Atoi(moves[indexStart:index]) // We convert the number to int
		if err != nil {
			panic("Digit parsing error")
		}
		index-- // Once we get the value, we go back to not miss a move

		// This two variables help us to go back if we find a wall
		lastCol := col
		lastRow := row

		for steps > 0 { // We move, knowing how many steps should we take and the direction
			err = makeMove(facing, &col, &row)
			if err != nil { // If we return an error, it means that we are trying to go through a wall, so we need to skip this step
				continue
			}
			currentTile := grid[row][col] // We get tile after moving
			if currentTile == -1 {        // Wrap around on the next step
				continue
			}
			if currentTile == 0 { // We are in a wall, which is imposible, go back to the last valid position
				col = lastCol
				row = lastRow
				break
			}
			if currentTile == 1 { // Valid position
				lastCol = col
				lastRow = row
			}
			steps--
		}
	}

	total := 1000*(row+1) + 4*(col+1) + facing
	fmt.Printf("Part 1 solution: %d\n", total)
}

func makeMove(facing int, col, row *int) error {
	// Moves, pretty much self explanatory
	if facing == dirs['R'] {
		*col++
		if *col >= len(grid[*row]) {
			*col = 0
		}
	}
	if facing == dirs['D'] {
		*row++
		if *row >= len(grid) {
			*row = 0
		}
		if *col >= len(grid[*row]) {
			return errors.New("Non existent position")
		}
	}
	if facing == dirs['L'] {
		*col--
		if *col < 0 {
			*col = len(grid[*row]) - 1
		}
	}
	if facing == dirs['U'] {
		*row--
		if *row < 0 {
			*row = len(grid) - 1
		}
		if *col >= len(grid[*row]) {
			return errors.New("Non existent position")
		}
	}
	return nil
}

func parseInput() {
	file, err := os.Open("input.txt")
	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		var row []int
		line := scanner.Text()
		if line == "" { // It means that next lines is the moves line
			scanner.Scan()
			moves = scanner.Text()
			break
		}

		// Here we parse the grid
		for i := 0; i < len(line); i++ {
			row = append(row, tiles[line[i]]) // We append each tile value to the row before appending it to the whole grid
		}
		grid = append(grid, row)
	}
}

func main() {
	parseInput()
	part1()
	// For part 2, the best shot is to manually parse the cube and calculate
	// Actually, I did print a cube given my input and calculated it manually, so no code for this one...
}
