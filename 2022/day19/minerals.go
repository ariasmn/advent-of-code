package main

import (
	"bufio"
	"fmt"
	"log"
	"math"
	"os"
)

// I did some tests and you could drastically improve the performance by not using objects, and using the map with a string cache key

type Blueprint struct {
	id       int
	ore      int
	clay     int
	obsidian [2]int
	geode    [2]int
}

type State struct { // This save the status of each mint, to make things easier when using BFS.
	time             int
	oreQuantity      int
	clayQuantity     int
	obsidianQuantity int
	geodeQuantity    int
	oreRobots        int
	clayRobots       int
	obsidianRobots   int
	geodeRobots      int
}

var blueprints []Blueprint // all the blueprints

func part1() {
	var quality int
	for _, blueprint := range blueprints {
		quality += blueprint.id * solve(blueprint, 24)
	}

	fmt.Printf("Part 1 solution: %d\n", quality)
}

func part2() {
	var total = 1
	for _, blueprint := range blueprints[:3] {
		total *= solve(blueprint, 32)
	}

	fmt.Printf("Part 2 solution: %d\n", total)
}

// Once again, using BFS to bruteforce
// Looking at some solutions in Reddit, this is not the best in terms of performance, but it works and is not unbearably slow
func solve(blueprint Blueprint, time int) int {
	visited := make(map[State]bool)
	initState := State{time, 0, 0, 0, 0, 1, 0, 0, 0} // Initial state. Everything is 0 except the ore robots, since we start with 1
	queue := []State{initState}
	var currentMax int
	var maxOreCost = maxSlice(blueprint.ore, blueprint.clay, blueprint.obsidian[0], blueprint.geode[0]) // This will hold the max ore we need to build a robot

	for len(queue) > 0 {
		currentState := queue[0]
		queue = queue[1:]

		if currentState.geodeQuantity > currentMax { // Remember best current max
			currentMax = currentState.geodeQuantity
		}
		if currentState.time == 0 || visited[currentState] { // Skip if already calculated
			continue
		}
		visited[currentState] = true

		newStates := generateStates(blueprint, currentState, maxOreCost) // New states based on the current states that we will add to the queue and calculate later
		queue = append(queue, newStates...)
	}

	return currentMax
}

// This function takes the current state and generates possible states
func generateStates(blueprint Blueprint, currentState State, maxOreCost int) []State {
	var states []State
	// Best move, so do nothing else
	if currentState.oreQuantity >= blueprint.geode[0] && currentState.obsidianQuantity >= blueprint.geode[1] {
		newState := State{
			time:             currentState.time - 1,
			oreQuantity:      currentState.oreQuantity - blueprint.geode[0] + currentState.oreRobots, // We have to substract the ore we used to create the new geode robot
			clayQuantity:     currentState.clayQuantity + currentState.clayRobots,
			obsidianQuantity: currentState.obsidianQuantity - blueprint.geode[1] + currentState.obsidianRobots, // Same with the obsidian
			geodeQuantity:    currentState.geodeQuantity + currentState.geodeRobots,
			oreRobots:        currentState.oreRobots,
			clayRobots:       currentState.clayRobots,
			obsidianRobots:   currentState.obsidianRobots,
			geodeRobots:      currentState.geodeRobots + 1, // We now have 1 more geode robot
		}
		return append(states, newState)
	}
	// Build obsidian robot if we have the resources AND we need more to build a geode robot
	if currentState.oreQuantity >= blueprint.obsidian[0] && currentState.clayQuantity >= blueprint.obsidian[1] && blueprint.geode[1] > currentState.obsidianRobots {
		newState := State{
			time:             currentState.time - 1,
			oreQuantity:      currentState.oreQuantity - blueprint.obsidian[0] + currentState.oreRobots,
			clayQuantity:     currentState.clayQuantity - blueprint.obsidian[1] + currentState.clayRobots,
			obsidianQuantity: currentState.obsidianQuantity + currentState.obsidianRobots,
			geodeQuantity:    currentState.geodeQuantity + currentState.geodeRobots,
			oreRobots:        currentState.oreRobots,
			clayRobots:       currentState.clayRobots,
			obsidianRobots:   currentState.obsidianRobots + 1,
			geodeRobots:      currentState.geodeRobots,
		}
		states = append(states, newState)
	}
	// Build clay robot if we have the resources AND we need more to build an obsidian robot
	if currentState.oreQuantity >= blueprint.clay && blueprint.obsidian[1] > currentState.clayRobots {
		newState := State{
			time:             currentState.time - 1,
			oreQuantity:      currentState.oreQuantity - blueprint.clay + currentState.oreRobots,
			clayQuantity:     currentState.clayQuantity + currentState.clayRobots,
			obsidianQuantity: currentState.obsidianQuantity + currentState.obsidianRobots,
			geodeQuantity:    currentState.geodeQuantity + currentState.geodeRobots,
			oreRobots:        currentState.oreRobots,
			clayRobots:       currentState.clayRobots + 1,
			obsidianRobots:   currentState.obsidianRobots,
			geodeRobots:      currentState.geodeRobots,
		}
		states = append(states, newState)
	}
	// Build ore robot if we have the resources AND we need more to build more robots
	if currentState.oreQuantity >= blueprint.ore && maxOreCost > currentState.oreRobots {
		newState := State{
			time:             currentState.time - 1,
			oreQuantity:      currentState.oreQuantity - blueprint.ore + currentState.oreRobots,
			clayQuantity:     currentState.clayQuantity + currentState.clayRobots,
			obsidianQuantity: currentState.obsidianQuantity + currentState.obsidianRobots,
			geodeQuantity:    currentState.geodeQuantity + currentState.geodeRobots,
			oreRobots:        currentState.oreRobots + 1,
			clayRobots:       currentState.clayRobots,
			obsidianRobots:   currentState.obsidianRobots,
			geodeRobots:      currentState.geodeRobots,
		}
		states = append(states, newState)
	}
	// Build nothing and collect resources only if more ore to build robots needed
	if maxOreCost >= currentState.oreQuantity {
		newState := State{
			time:             currentState.time - 1,
			oreQuantity:      currentState.oreQuantity + currentState.oreRobots,
			clayQuantity:     currentState.clayQuantity + currentState.clayRobots,
			obsidianQuantity: currentState.obsidianQuantity + currentState.obsidianRobots,
			geodeQuantity:    currentState.geodeQuantity + currentState.geodeRobots,
			oreRobots:        currentState.oreRobots,
			clayRobots:       currentState.clayRobots,
			obsidianRobots:   currentState.obsidianRobots,
			geodeRobots:      currentState.geodeRobots,
		}
		states = append(states, newState)
	}

	return states
}

func maxSlice(args ...int) int {
	larger := math.MinInt
	for _, element := range args {
		if element > larger {
			larger = element
		}
	}

	return larger
}

func parseBlueprints() {
	file, err := os.Open("input.txt")
	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()
		blueprint := Blueprint{}
		fmt.Sscanf(
			line,
			"Blueprint %d: Each ore robot costs %d ore. Each clay robot costs %d ore. Each obsidian robot costs %d ore and %d clay. Each geode robot costs %d ore and %d obsidian",
			&blueprint.id,
			&blueprint.ore,
			&blueprint.clay,
			&blueprint.obsidian[0],
			&blueprint.obsidian[1],
			&blueprint.geode[0],
			&blueprint.geode[1],
		)

		blueprints = append(blueprints, blueprint)
	}
}

func main() {
	parseBlueprints()
	part1()
	part2()
}
