package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
)

type sections struct {
	start int
	end   int
}

func (sections1 sections) contains(sections2 sections) bool {
	return sections1.start <= sections2.start && sections1.end >= sections2.end
}

func (sections1 sections) overlap(sections2 sections) bool {
	return sections1.start <= sections2.start && sections1.end >= sections2.start
}

func part1() {
	file, err := os.Open("input.txt")

	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)

	totalContains := 0

	for scanner.Scan() {
		line := scanner.Text()
		sections1, sections2 := parseSections(line)

		if sections1.contains(sections2) || sections2.contains(sections1) {
			totalContains++
		}
	}

	fmt.Printf("Part 1 result: %d\n", totalContains)
}

func part2() {
	file, err := os.Open("input.txt")

	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)

	totalOverlaps := 0

	for scanner.Scan() {
		line := scanner.Text()
		sections1, sections2 := parseSections(line)

		if sections1.overlap(sections2) || sections2.overlap(sections1) {
			totalOverlaps++
		}
	}

	fmt.Printf("Part 2 result: %d\n", totalOverlaps)
}

func parseSections(line string) (sections, sections) {
	var sections1, sections2 sections
	fmt.Sscanf(line, "%d-%d,%d-%d", &sections1.start, &sections1.end, &sections2.start, &sections2.end)

	return sections1, sections2
}

func main() {
	part1()
	part2()
}
