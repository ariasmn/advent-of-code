package main

import (
	"bufio"
	"errors"
	"fmt"
	"log"
	"math"
	"os"
)

// Using BFS because it's the easiest for me to implement

// Using a coord struct, since I didn't use it in the day 9 problem, just to change things for a bit
type Coord struct {
	x int
	y int
}

func part1() {
	grid := parseGrid()
	var start Coord
	var end Coord

	for i := range grid {
		for j := range grid[i] {
			if grid[i][j] == 'S' { // Find the start
				start = Coord{i, j}
			}
			if grid[i][j] == 'E' { // Find the end
				end = Coord{i, j}
			}
		}
	}

	// Remove "unknown" chars (S and E) with computable chars
	// We use the "lowest" and the "highest"
	grid[start.x][start.y] = 'a'
	grid[end.x][end.y] = 'z'

	solution, _ := findFewestSteps(grid, start, end)

	fmt.Printf("Part 1 solution: %d\n", solution)
}

func part2() {
	grid := parseGrid()
	var starts []Coord // We now have multiple starting points
	var end Coord

	for i := range grid {
		for j := range grid[i] {
			if grid[i][j] == 'S' || grid[i][j] == 'a' { // Find the starts, either an "S" or elevation "a"
				grid[i][j] = 'a' // In case it's 'S', we modify it and use 'a'
				starts = append(starts, Coord{i, j})
			}
			if grid[i][j] == 'E' { // Find the end
				grid[i][j] = 'z'
				end = Coord{i, j}
			}
		}
	}

	minSteps := math.MaxInt
	for _, start := range starts {
		currentSteps, err := findFewestSteps(grid, start, end)
		if err != nil {
			continue
		}
		if currentSteps < minSteps {
			minSteps = currentSteps
		}
	}

	fmt.Printf("Part 2 solution: %d\n", minSteps)
}

// The BFS implementation (Queue)
func findFewestSteps(grid [][]rune, start Coord, end Coord) (int, error) {
	visited := make(map[Coord]int)                          // Insert the distance between the starting coord and each visited coord
	visited[start] = 0                                      // The starting point is already visited
	neighbours := []Coord{{1, 0}, {0, 1}, {-1, 0}, {0, -1}} // Possible directions to go from one coord
	queue := []Coord{start}

	for len(queue) > 0 {
		currentCoord := queue[0] // Get current coord we are visiting
		queue = queue[1:]        // Remove it from the queue

		if currentCoord == end {
			return visited[currentCoord], nil
		}

		for _, n := range neighbours {
			nextCoord := Coord{currentCoord.x + n.x, currentCoord.y + n.y}

			if _, isVisited := visited[nextCoord]; isVisited {
				continue
			}
			if nextCoord.x < 0 || nextCoord.x >= len(grid) || nextCoord.y < 0 || nextCoord.y >= len(grid[0]) { // Non possible cases
				continue
			}

			// We get the values (in ASCII since we are working with runes)
			// If the next coord to visit is higher than 2, we can't go there so we skip it
			nextCoordVal := grid[nextCoord.x][nextCoord.y]
			currentCoordVal := grid[currentCoord.x][currentCoord.y]

			if nextCoordVal > currentCoordVal && nextCoordVal-currentCoordVal > 1 {
				continue
			}

			visited[nextCoord] = visited[currentCoord] + 1
			queue = append(queue, nextCoord)
		}
	}

	return -1, errors.New("Couldn't find a path")
}

func parseGrid() [][]rune {
	file, err := os.Open("input.txt")
	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)
	grid := make([][]rune, 0)

	for scanner.Scan() {
		line := scanner.Text()
		grid = append(grid, []rune(line))
	}

	return grid
}

func main() {
	part1()
	part2()
}
