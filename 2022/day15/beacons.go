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

var sensors = map[Coord]int{}  // We are saving the sensors' position and the distance to the closest beacon
var beacons = map[Coord]bool{} // Saving beacons' position too

func part1() {
	var lineY int = 2_000_000    // Line that we want to check
	var checked = map[int]bool{} // Map that contains that x coord within the lineY that cannot contain a beacon
	// We calculate the different intervals for the Y line for each sensor
	// For reference: https://youtu.be/w7m48_uCvWI?t=335 explains it perfectly
	for sensor := range sensors {
		intDist := intAbs(sensors[sensor]) - intAbs(lineY-sensor.y) // this is the intervals we need to check both left and right from sensor's x pos
		for x := sensor.x - intDist; x <= sensor.x+intDist; x++ {   // We go through the interval and check
			if !beacons[Coord{x, lineY}] { // If the beacon is already in there, we don't have to add it obviously
				checked[x] = true
			}
		}
	}

	fmt.Printf("Part 1 solution: %d\n", len(checked))
}

func part2() {
	var maxGrid int = 4_000_000
	var free Coord

	defer func() {
		fmt.Printf("Part 2 solution: %d\n", free.x*4_000_000+free.y)
	}()

	for y := 0; y <= maxGrid; y++ {
	main:
		for x := 0; x <= maxGrid; x++ {
			for sensor, distance := range sensors {
				distanceToSensor := intAbs(sensor.x-x) + intAbs(sensor.y-y) // We calculate the distance between the coord (x, y) we are checking and the sensor we are looping
				if distanceToSensor <= distance {                           // This means that the coord we are checking is a blocked one
					x = sensor.x + (distance - intAbs(y-sensor.y)) // We advance X so we skip blocked positions
					continue main
				}
			}
			free = Coord{x, y}
			return
		}
	}
}

func parseZone() {
	file, err := os.Open("input.txt")
	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		var sensor, beacon Coord
		fmt.Sscanf(scanner.Text(), "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d", &sensor.x, &sensor.y, &beacon.x, &beacon.y)
		sensors[sensor] = dist(sensor, beacon)
		beacons[beacon] = true
	}
}

func dist(pointA, pointB Coord) int { // Returns distance between two coords
	return intAbs(pointA.x-pointB.x) + intAbs(pointA.y-pointB.y)
}

func intAbs(number int) int {
	if number < 0 {
		return -number
	}

	return number
}

func main() {
	parseZone()
	part1()
	part2()
}
