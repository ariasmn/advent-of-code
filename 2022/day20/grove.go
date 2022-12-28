package main

import (
	"bufio"
	"container/ring"
	"fmt"
	"log"
	"os"
	"strconv"
)

// This is pretty easy if you think about it as a circular list: https://www.geeksforgeeks.org/circular-linked-list/
// And actually, Go's standard library provides a package to work with them. Example: https://gist.github.com/hartfordfive/77f2e6de9abd3be4ba66123b9e526f0f
// Is important to understant that a "Ring" is actually an element of the circular list, and not the circular list itself

var arrangement []int
var positions = map[int]*ring.Ring{} // Initial position of each value in the circular list, referencing to the ring
var zeroIndex = (*ring.Ring)(nil)    // Reference to the 0 element

func part1() {
	parseEncrypted(1)
	generateCircularList()
	for index, number := range arrangement { // We start mixing the numbers on the circular list
		rElement := positions[index].Prev()  // We get the previous element, because we will remove the next one
		unlinked := rElement.Unlink(1)       // We unlink it from the circular list and store it here
		rElement.Move(number).Link(unlinked) // We move the pointer to the where the number indicates and insert the unlinked element here, to simulate the movement
	}

	// Now that each value is in the desired position, we calculate the "grove coordinates"
	// We use "zeroIndex" since we need to start couting from the 0 value, that's why we save it
	sum := zeroIndex.Move(1000).Value.(int) + zeroIndex.Move(2000).Value.(int) + zeroIndex.Move(3000).Value.(int)

	fmt.Printf("Part 1 solution: %d\n", sum)
}

func part2() {
	parseEncrypted(811589153)
	generateCircularList()
	for i := 0; i < 10; i++ {
		for index, number := range arrangement {
			rElement := positions[index].Prev()
			unlinked := rElement.Unlink(1)
			rElement.Move(number % (len(arrangement) - 1)).Link(unlinked) // https://www.reddit.com/r/adventofcode/comments/zqezkn/comment/j10gg7a/?utm_source=share&utm_medium=web2x&context=3
		}
	}

	sum := zeroIndex.Move(1000).Value.(int) + zeroIndex.Move(2000).Value.(int) + zeroIndex.Move(3000).Value.(int)

	fmt.Printf("Part 2 solution: %d\n", sum)
}

func generateCircularList() {
	positions = map[int]*ring.Ring{} // Clear the positions, to do both parts

	rs := ring.New(len(arrangement)) // We create a ring which length is the number of values
	for index, value := range arrangement {
		if value == 0 { // We store the reference of the element 0, since we should start calculating coordinates here
			zeroIndex = rs
		}
		rs.Value = value
		positions[index] = rs
		rs = rs.Next()
	}
}

func parseEncrypted(decryptionKey int) {
	arrangement = nil
	file, err := os.Open("input.txt")
	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		numString := scanner.Text()
		num, _ := strconv.Atoi(numString)
		arrangement = append(arrangement, num*decryptionKey) // We parse the numbers using the encryption key
	}
}

func main() {
	part1()
	part2()
}
