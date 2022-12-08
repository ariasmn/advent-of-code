package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"sort"
	"strconv"
	"strings"
)

// Bit of "cheating" in this one.
// This problem is related to the DFS algorithm.
// But since the input is already starting from the root (/), we can skip the construction of the node tree.

var parentDirSizes []int64
var dirSizes []int64

func part1() {
	file, err := os.Open("input.txt")

	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)

	parentDirSizes = nil
	var total int64

	for scanner.Scan() {
		line := scanner.Text()

		if strings.Contains(line, "$ ls") || strings.Contains(line, "dir") {
			continue
		}

		if strings.Contains(line, "$ cd") {
			dir := strings.Fields(line)[2]

			if dir != ".." {
				parentDirSizes = append(parentDirSizes, 0) // Get into the new directory
				continue
			}

			// If going back with `..`, do...
			lastVisitedDirSize := parentDirSizes[len(parentDirSizes)-1]

			// Sum to total if the dir is inside the max threshold
			if lastVisitedDirSize <= 100000 {
				total += lastVisitedDirSize
			}

			parentDirSizes[len(parentDirSizes)-2] += lastVisitedDirSize // Sum the inside directory
			parentDirSizes = parentDirSizes[:len(parentDirSizes)-1]     // Remove last sum

		}

		file := strings.Fields(line)[0]
		size, _ := strconv.ParseInt(file, 10, 64)
		parentDirSizes[len(parentDirSizes)-1] += size // Add size to the current directory
	}

	fmt.Printf("Part 1 solution: %d\n", total)
}

// Same as part1, but storing each dir in another slice before adding to parent and removing it
func part2() {
	file, err := os.Open("input.txt")

	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)

	parentDirSizes = nil

	for scanner.Scan() {
		line := scanner.Text()

		if strings.Contains(line, "$ ls") || strings.Contains(line, "dir") {
			continue
		}

		if strings.Contains(line, "$ cd") {
			dir := strings.Fields(line)[2]

			if dir != ".." {
				parentDirSizes = append(parentDirSizes, 0) // Get into the new directory
				continue
			}

			// If going back with `..`, do...
			calculateRealDirSizes()
		}

		file := strings.Fields(line)[0]
		size, _ := strconv.ParseInt(file, 10, 64)
		parentDirSizes[len(parentDirSizes)-1] += size // Add size to the current directory
	}

	for len(parentDirSizes) > 1 {
		calculateRealDirSizes()
	}

	dirSizes = append(dirSizes, parentDirSizes...) // Only the root size left, we append it
	sort.Slice(dirSizes, func(i, j int) bool {
		return dirSizes[j] < dirSizes[i]
	})

	used := dirSizes[0] // The first one in dirSize is the root (/), which is the total used
	free := 70000000 - used

	var sizeNeeded int64
	for _, size := range dirSizes {
		if free+size >= 30000000 {
			sizeNeeded = size
			continue
		}
	}

	fmt.Printf("Part 2 solution: %d\n", sizeNeeded)
}

func calculateRealDirSizes() {
	lastVisitedDirSize := parentDirSizes[len(parentDirSizes)-1]

	parentDirSizes[len(parentDirSizes)-2] += lastVisitedDirSize // Sum the inside directory
	dirSizes = append(dirSizes, lastVisitedDirSize)             // Add to individual slice before removing
	parentDirSizes = parentDirSizes[:len(parentDirSizes)-1]     // Remove last from the parents slice
}

func main() {
	part1()
	part2()
}
