package main

import (
	"bufio"
	"fmt"
	"log"
	"math"
	"os"
)

var grid = map[Cube]bool{} // This will hold all the set cubes in the grid

var minX, minY, minZ int = math.MaxInt, math.MaxInt, math.MaxInt // min coords values
var maxX, maxY, maxZ int = math.MinInt, math.MinInt, math.MinInt // max coords values. Both used to check if a neighbor is out of the grid

type Cube struct {
	x int
	y int
	z int
}

func part1() {
	var exposed int = 0
	for cube := range grid { // We check all cubes
		neighbors := generateNeighbors(cube) // Generate cube's neighbors
		for _, neighborCube := range neighbors {
			if _, ok := grid[neighborCube]; !ok { // If the neighbor is not in the grid, it means that its side is not connected
				exposed++
			}
		}
	}

	fmt.Printf("Part 1 solution: %d\n", exposed)
}

// The idea for part2 is to check if each cube's neighbor can exit
// If it can't exit, it means it's an air pocket, so we don't have to count it as exposed
func part2() {
	var exposed int = 0
	for cube := range grid {
		neighbors := generateNeighbors(cube)
		for _, neighborCube := range neighbors {
			if canExit(neighborCube) {
				exposed++
			}
		}
	}

	fmt.Printf("Part 2 solution: %d\n", exposed)
}

// Using BFS to check if a cube can get outside the grid
// If it does, it means that it's not trapped, so it's NOT an air pocket
func canExit(cube Cube) bool {
	visited := make(map[Cube]bool)
	queue := []Cube{cube}

	for len(queue) > 0 {
		currentCube := queue[0] // Get cube we are checking
		queue = queue[1:]       // Remove it from the queue

		if visited[currentCube] { // If already seen, skip
			continue
		}
		visited[currentCube] = true         // Add it to the seen map
		if _, ok := grid[currentCube]; ok { // If a cube exists in that position, skip
			continue
		}
		if inAir(currentCube) { // If in air, it means it can exit, so return true
			return true
		}
		for _, neighbor := range generateNeighbors(currentCube) {
			queue = append(queue, neighbor) // We append the neighbors to the queue
		}
	}

	return false // Means that it cannot exit, so return false
}

func inAir(cube Cube) bool {
	if cube.x > maxX || cube.x < minX {
		return true
	}
	if cube.y > maxY || cube.y < minY {
		return true
	}
	if cube.z > maxZ || cube.z < minZ {
		return true
	}

	return false
}

func generateNeighbors(cube Cube) []Cube {
	var neighbors []Cube
	// There should be a cleaner way of doing this but w/e
	neighbors = append(neighbors, Cube{cube.x - 1, cube.y, cube.z})
	neighbors = append(neighbors, Cube{cube.x + 1, cube.y, cube.z})
	neighbors = append(neighbors, Cube{cube.x, cube.y - 1, cube.z})
	neighbors = append(neighbors, Cube{cube.x, cube.y + 1, cube.z})
	neighbors = append(neighbors, Cube{cube.x, cube.y, cube.z - 1})
	neighbors = append(neighbors, Cube{cube.x, cube.y, cube.z + 1})

	return neighbors
}
func parseGrid() {
	file, err := os.Open("input.txt")
	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()
		cube := Cube{}
		fmt.Sscanf(line, "%d,%d,%d", &cube.x, &cube.y, &cube.z) // Parse cube
		grid[cube] = true                                       // Add it to the grid

		// Get max and min coords while parsing the grid
		if cube.x < minX {
			minX = cube.x
		}
		if cube.x > maxX {
			maxX = cube.x
		}
		if cube.y < minY {
			minY = cube.y
		}
		if cube.y > minY {
			maxY = cube.y
		}
		if cube.z < minZ {
			minZ = cube.z
		}
		if cube.z > maxZ {
			maxZ = cube.z
		}
	}
}

func main() {
	parseGrid()
	part1()
	part2()
}
