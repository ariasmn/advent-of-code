package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
)

// Again, we assume that we know the row and column beforehand
const rowLength int = 99
const columnLength int = 99

var forest [rowLength][columnLength]int

func part1() {
	file, err := os.Open("input.txt")
	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)
	row := 0
	totalVisible := 0

	for scanner.Scan() {
		line := scanner.Text()

		for index, height := range line {
			forest[row][index] = int(height)
		}
		row++
	}

	for iRow, row := range forest {
		for iColumn, _ := range row {
			if iRow == 0 || iColumn == 0 || iRow == rowLength || iColumn == columnLength {
				totalVisible++
				continue
			}

			if checkIfVisible(iRow, iColumn) {
				totalVisible++
			}
		}
	}

	fmt.Printf("Part 1 solution: %d\n", totalVisible)
}

func part2() {
	file, err := os.Open("input.txt")
	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)
	row := 0

	for scanner.Scan() {
		line := scanner.Text()

		for index, height := range line {
			forest[row][index] = int(height)
		}
		row++
	}

	var highestScenic int

	for iRow, row := range forest {
		for iColumn, _ := range row {
			scenicRow := calcScenicRow(iRow, iColumn)
			scenicColumn := calcScenicColumn(iRow, iColumn)

			treeScenic := scenicRow * scenicColumn
			if treeScenic > highestScenic {
				highestScenic = treeScenic
			}
		}
	}

	fmt.Printf("Part 2 solution: %d\n", highestScenic)
}

func checkIfVisible(row, column int) bool {

	if checkRow(row, column) {
		return true
	}

	if checkColumn(row, column) {
		return true
	}

	return false
}

func checkRow(row, column int) bool {
	height := forest[row][column]
	left := true
	right := true

	// The behaviour is always the same.
	// Check all trees, and if the view is blocked in the direction we are checking, mark it as "false" and break the loop

	for i := column - 1; i >= 0; i-- {
		if forest[row][i] >= height {
			left = false
			break
		}
	}

	for i := column + 1; i < columnLength; i++ {
		if forest[row][i] >= height {
			right = false
			break
		}
	}

	return left || right
}

func checkColumn(row, column int) bool {
	height := forest[row][column]
	top := true
	bottom := true

	// The behaviour is always the same.
	// Check all trees, and if the view is blocked in the direction we are checking, mark it as "false" and break the loop

	for i := row - 1; i >= 0; i-- {
		if forest[i][column] >= height {
			top = false
			break
		}
	}

	for i := row + 1; i < rowLength; i++ {
		if forest[i][column] >= height {
			bottom = false
			break
		}
	}

	return top || bottom
}

func calcScenicRow(row, column int) int {
	height := forest[row][column]
	left := 0
	right := 0

	// If the tree's height is higher, keep checking other trees
	// If it's the same or lower, we still can see it, but we stop checking for other trees

	for i := column - 1; i >= 0; i-- {
		if forest[row][i] >= height {
			left++
			break
		}

		if forest[row][i] < height {
			left++
		}
	}

	for i := column + 1; i < columnLength; i++ {
		if forest[row][i] >= height {
			right++
			break
		}

		if forest[row][i] < height {
			right++
		}
	}

	return left * right
}

func calcScenicColumn(row, column int) int {
	height := forest[row][column]
	top := 0
	bottom := 0

	// If the tree's height is higher, keep checking other trees
	// If it's the same or lower, we still can see it, but we stop checking for other trees

	for i := row - 1; i >= 0; i-- {
		if forest[i][column] >= height {
			top++
			break
		}

		if forest[i][column] < height {
			top++
		}
	}

	for i := row + 1; i < rowLength; i++ {
		if forest[i][column] >= height {
			bottom++
			break
		}

		if forest[i][column] < height {
			bottom++
		}
	}

	return top * bottom
}

func main() {
	part1()
	part2()
}
