package main

import (
	"bufio"
	"encoding/json"
	"fmt"
	"log"
	"os"
	"sort"
	"strings"
)

func part1() {
	file, err := os.Open("input.txt")
	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)
	var index, sum int

	for scanner.Scan() {
		line := strings.TrimSpace(scanner.Text())

		if line == "\n" || line == "" { // Skip if new line
			continue
		}

		first := parsePacket(line) // Parse current packet
		scanner.Scan()
		line = strings.TrimSpace(scanner.Text()) // Advance scanner and scan next line, which sould be second packet
		second := parsePacket(line)              // Parse second packet

		if comparePackets(first, second) <= 0 {
			sum += index + 1
		}

		index++
	}

	fmt.Printf("Part 1 solution: %d\n", sum)
}

func part2() {
	file, err := os.Open("input.txt")
	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)
	var packets []any

	// Parse the packets as we did in part1
	for scanner.Scan() {
		line := strings.TrimSpace(scanner.Text())

		if line == "\n" || line == "" { // Skip if new line
			continue
		}

		first := parsePacket(line) // Parse current packet
		scanner.Scan()
		line = strings.TrimSpace(scanner.Text()) // Advance scanner and scan next line, which sould be second packet
		second := parsePacket(line)              // Parse second packet

		packets = append(packets, first, second) // We add them into an slice
	}

	packets = append(packets, []any{[]any{float64(2)}}, []any{[]any{float64(6)}}) // Add divider packets
	sort.Slice(packets, func(i, j int) bool {                                     // Sort it using the compare function
		return comparePackets(packets[i], packets[j]) < 0 // If less than 0, it means that the `i` is lower than `j`
	})

	decoder := 1
	for index, packet := range packets {
		if fmt.Sprint(packet) == "[[2]]" || fmt.Sprint(packet) == "[[6]]" {
			decoder *= index + 1
		}
	}

	fmt.Printf("Part 2 solution: %d\n", decoder)
}

// Props to https://github.com/mnml/aoc/blob/main/2022/13/1.go, my compare function was a mess before I saw this implementation
func comparePackets(first, second any) int {
	firstPackets, firstOk := first.([]any) // Check if packet or a number. Stored in the '_Ok' variable in both cases
	secondPackets, secondOk := second.([]any)

	if !firstOk && !secondOk { // None of them are packets, we can calculate normally
		return int(first.(float64) - second.(float64))
	}

	// If any of variables is not a packet, we create one from the number to compare both equally
	if !firstOk {
		firstPackets = []any{first}
	}

	if !secondOk {
		secondPackets = []any{second}
	}

	// Use recursion
	for i := 0; i < len(firstPackets) && i < len(secondPackets); i++ {
		if c := comparePackets(firstPackets[i], secondPackets[i]); c != 0 {
			return c
		}
	}

	return len(firstPackets) - len(secondPackets) // Running out of items
}

func parsePacket(line string) any {
	var parsed any

	err := json.Unmarshal([]byte(line), &parsed)
	if err != nil {
		log.Fatal(err)
	}

	return parsed
}

func main() {
	part1()
	part2()
}
