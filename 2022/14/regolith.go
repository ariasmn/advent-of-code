package main

import (
	"bufio"
	"fmt"
	"log"
	"math"
	"os"
	"strings"
)

type Coord struct {
	x int
	y int
}

var cave = map[Coord]rune{}

var maxY int               // Max Y coord. Needed as a limit to check when pouring the sand.
var maxX int               // Max X coord. Needed in the second part to add the floor.
var minX int = math.MaxInt // Min X coord. Needed in the second part to add the floor.

func part1() {
	parseCave()
	stop := false         // Flag that checks when to stop the sound pouring
	var countSand int = 0 // Number of "sands"

	for !stop {
		startSand := Coord{500, 0} // We pour sand from the starting point when each iteration is complete
		for {
			cave[startSand] = 1       // We are checking if it's rock or air, so we assign the lowest value, since '#' (35) will be greater
			if startSand.y+1 > maxY { // If already in the y limit, we stop pouring sand and finish
				stop = true
				break
			}
			if cave[Coord{startSand.x, startSand.y + 1}] < '#' { // The first possible move: sand keeps going down
				startSand.y++
				continue
			}
			if cave[Coord{startSand.x - 1, startSand.y + 1}] < '#' { // If the sand can't go down anymore, it tries to go left and down
				startSand.x--
				startSand.y++
				continue
			}
			if cave[Coord{startSand.x + 1, startSand.y + 1}] < '#' { // If not, it tries to go right and down
				startSand.x++
				startSand.y++
				continue
			}

			// If the sand can't move more, it means that it comes to rest
			cave[startSand] = 'o'
			countSand++
			break
		}
	}

	fmt.Printf("Part 1 solution: %d\n", countSand)
}

func part2() {
	cave = make(map[Coord]rune) // Clear and parse again
	parseCave()

	// We add the floor. Its from -infinity to +infinity, but setting a big amount is more than enough
	// This is needed so the sand doesn't get to rest outside the cave limits
	for i := minX - 300; i < maxX+300; i++ {
		cave[Coord{i, maxY + 2}] = '#'
	}
	var countSand int = 0

	for {
		newSand := Coord{500, 0}
		if cave[newSand] == 'o' { // We don't have a stop flag anymore, we stop when the sand rests on the starting pouring point
			break
		}
		for {
			cave[newSand] = '"'
			if cave[Coord{newSand.x, newSand.y + 1}] < '#' {
				newSand.y++
			} else if cave[Coord{newSand.x - 1, newSand.y + 1}] < '#' {
				newSand.y++
				newSand.x--
			} else if cave[Coord{newSand.x + 1, newSand.y + 1}] < '#' {
				newSand.y++
				newSand.x++
			} else {
				cave[newSand] = 'o'
				countSand++
				break
			}
		}
	}

	fmt.Printf("Part 2 solution: %d\n", countSand)
}

func parseCave() {
	file, err := os.Open("input.txt")
	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)

	for scanner.Scan() {
		paths := strings.Split(scanner.Text(), " -> ") // Split each line into the paths
		for index := range paths[:len(paths)-1] {
			var path1, path2 Coord
			fmt.Sscanf(paths[index], "%d,%d", &path1.x, &path1.y)
			fmt.Sscanf(paths[index+1], "%d,%d", &path2.x, &path2.y)
			updateBounds(path1, path2)
			placeRock(path1, path2)
		}
	}

}

func updateBounds(path1, path2 Coord) {
	// Getting the max y value
	if path1.y > maxY {
		maxY = path1.y
	}
	if path2.y > maxY {
		maxY = path2.y
	}

	// Getting the max x value
	if path1.x > maxX {
		maxX = path1.x
	}
	if path2.x > maxX {
		maxX = path2.x
	}

	// Getting the min x value
	if path1.x < minX {
		minX = path1.x
	}
	if path2.x < minX {
		minX = path2.x
	}
}

func placeRock(path1, path2 Coord) {
	cave[path1] = '#' // We put a rock in the first path coord

	if path1 == path2 { // If both path are the same, we exit the function since all rocks are placed
		return
	}

	// We are using this function recursively
	// We need to pass a new coord depending on the cases
	// The cases are:
	if path1.x == path2.x && path1.y > path2.y { // Same x coord, y coord should move down.
		placeRock(Coord{path1.x, path1.y - 1}, path2)
	}

	if path1.x == path2.x && path1.y < path2.y { // Same x coord, y should move up.
		placeRock(Coord{path1.x, path1.y + 1}, path2)
	}

	if path1.x > path2.x && path1.y == path2.y { // x coord should move left, same y coord.
		placeRock(Coord{path1.x - 1, path1.y}, path2)
	}

	if path1.x < path2.x && path1.y == path2.y { // x coord should move right, same y coord.
		placeRock(Coord{path1.x + 1, path1.y}, path2)
	}
}

func main() {
	part1()
	part2()
}
