package main

// Being honest, this one was super hard for me, so I look up over some solutions to pick some references
// Please, go check https://github.com/carolinasolfernandez/advent-of-code-2022/blob/main/16/main.go

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"strings"
)

type Valve struct {
	name     string // For easier accessability later
	flowRate int
	leadTo   []string
}

var valves = map[string]Valve{}          // All valves
var activeValves = map[string]bool{}     // Active valves, meaning, those which flow rate is greater than 0
var shortestDistances = map[string]int{} // Shortest distances between valves. Possible since the whole graph is traversal

func part1() {
	fmt.Printf("Part 1 solution: %d\n", getMaxPressure(valves["AA"], 31))
}

func part2() {
	fmt.Printf("Part 2 solution: %d\n", getMaxPressureWithHelp(valves["AA"], valves["AA"], 26, 27))
}

// This is a slightly optimized bruteforce
// We calculate every possibility and we always save the best one to return it
// Passing the activeValves as value since we need a clean copy on each iteration of the recursion
func getMaxPressure(valve Valve, timeLeft int) int {
	timeLeft-- // We decrease the time
	if timeLeft <= 0 {
		return 0
	}
	maxPressure := 0 // Starting pressure is 0
	for current, visited := range activeValves {
		if visited { // We have already open this valve, so we skip it
			continue
		}
		activeValves[current] = true                                                 // We mark it as already visited
		distanceTime := shortestDistances[formatValvesAsString(valve.name, current)] // We get the distance from current valve to the activeValves valve we want to visit
		valvePressure := getMaxPressure(valves[current], timeLeft-distanceTime)

		if valvePressure > maxPressure {
			maxPressure = valvePressure
		}
		activeValves[current] = false // Once we've calculated the maxPressure, we active it again for further calculations
	}
	return maxPressure + valve.flowRate*timeLeft
}

// The idea behind this is to keep track of both person's and elephant's valve opening and time
// Since we use recursion, we take turns each time: first elephant, then user, and repeat till all the possibilities are completed
// We could say that valve and time 1 represents the current action, while valve and time 2 represents the next action to do
// And in each iteration of the recursion, both get interchanged, so valve2/time2 becomes valve1/time1 and viceversa
func getMaxPressureWithHelp(valve1, valve2 Valve, time1, time2 int) int {
	time2--
	if time2 <= 0 {
		return 0
	}
	maxPressure := 0
	for current, visited := range activeValves {
		if visited {
			continue
		}
		activeValves[current] = true
		distanceTime := shortestDistances[formatValvesAsString(valve1.name, current)]
		valvePressure := getMaxPressureWithHelp(valve2, valves[current], time2, time1-distanceTime)

		if valvePressure > maxPressure {
			maxPressure = valvePressure
		}
		activeValves[current] = false
	}
	return maxPressure + valve2.flowRate*time2
}

// We generate shortest distance between valves, using BFS for the connected graph
func generateShortestDistances() {
	for _, valve := range valves {
		visited := map[string]int{}   // Visited valves and distance from the valve we are looping
		visited[valve.name] = 0       // The starting point is already visited
		queue := []string{valve.name} // We start the BFS queue

		for len(queue) > 0 {
			currentValve := queue[0] // Get current valve we are visiting
			queue = queue[1:]        // Remove it from the queue

			for _, neighbor := range valves[currentValve].leadTo { // We check each valve's neighbor
				if _, ok := visited[neighbor]; ok { // If already visited from this valve, we skip it
					continue
				}
				visited[neighbor] = visited[currentValve] + 1 // The visited neighbor is the last distance + 1
				shortestDistances[formatValvesAsString(valve.name, neighbor)] = visited[currentValve] + 1
				queue = append(queue, neighbor) // We append next neighbor to queue
			}
		}
	}
}

func parseValves() {
	file, err := os.Open("input.txt")
	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		var valve Valve
		var currentTunel string
		splitLine := strings.Split(scanner.Text(), "; ")
		fmt.Sscanf(splitLine[0], "Valve %s has flow rate=%d", &currentTunel, &valve.flowRate)

		tunnel := strings.ReplaceAll(splitLine[1], ", ", ",") // Remove spaces between commas to later range the lead valves
		leadTo := strings.Split(tunnel, " ")                  // We have the valves in here only
		leadTo = strings.Split(leadTo[4], ",")                // We split them by comma to have them as an slice

		valve.leadTo = leadTo
		valve.name = currentTunel
		valves[currentTunel] = valve
	}
}

func parseActiveValves() {
	for _, valve := range valves {
		if valve.flowRate > 0 {
			activeValves[valve.name] = false // We set it to false because we'll change to true once we visit it in the calculation
		}
	}
}

func formatValvesAsString(origin, neighbor string) string {
	// return fmt.Sprintf("%s-%s", origin, neighbor)
	return origin + "->" + neighbor // Using Sprintf makes the program hangs... Check https://medium.com/swlh/bad-go-frivolous-sprintf-2ad28fedf1a0
}

func main() {
	parseValves()
	generateShortestDistances()
	parseActiveValves()
	part1()
	part2()
}
