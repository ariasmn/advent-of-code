package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"strings"
)

type stack struct {
	crates []rune
}

func (s *stack) add(char rune) {
	s.crates = append(s.crates, char)
}

func (s *stack) addLast(char rune) {
	s.crates = append([]rune{char}, s.crates...)
}

func (s *stack) extract(howMany int) (r []rune) {
	extracted := s.crates[len(s.crates)-howMany : len(s.crates)]
	s.crates = s.crates[:len(s.crates)-howMany]

	return extracted
}

var stacks = make([]stack, 9)

func part1() {
	file, err := os.Open("input.txt")

	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)
	stacks = make([]stack, 9)

	for scanner.Scan() {
		line := scanner.Text()

		if strings.Contains(line, "[") {
			parseStacks(line)
		}
		if strings.Contains(line, "move") {
			howMany, from, to := parseMove(line)
			for i := 0; i < howMany; i++ {
				cratesLeft := len(stacks[from].crates)
				if cratesLeft > 0 {
					stacks[to].add(stacks[from].crates[cratesLeft-1])
					stacks[from].extract(1)
				}
			}
		}
	}

	fmt.Println("Part 1 solution:")
	for index, stack := range stacks {
		last := len(stack.crates) - 1
		fmt.Printf("Stack %d top crate is %s\n", index+1, string(stack.crates[last]))
	}
}

func part2() {
	file, err := os.Open("input.txt")

	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)
	stacks = make([]stack, 9)

	for scanner.Scan() {
		line := scanner.Text()

		if strings.Contains(line, "[") {
			parseStacks(line)
		}

		if strings.Contains(line, "move") {
			howMany, from, to := parseMove(line)
			cratesLeft := len(stacks[from].crates)
			if cratesLeft > 0 {
				if howMany > cratesLeft {
					howMany = cratesLeft
				}

				toMove := stacks[from].extract(howMany)
				stacks[to].crates = append(stacks[to].crates, toMove...)
			}
		}
	}

	fmt.Println("Part 2 solution:")
	for index, stack := range stacks {
		last := len(stack.crates) - 1
		fmt.Printf("Stack %d top crate is %s\n", index+1, string(stack.crates[last]))
	}

}

func parseStacks(line string) {
	for index, char := range line {
		if char != ' ' && char != '[' && char != ']' {
			stacks[index/4].addLast(char)
		}
	}
}

func parseMove(line string) (int, int, int) {
	var howMany, from, to int
	fmt.Sscanf(line, "move %d from %d to %d", &howMany, &from, &to)

	return howMany, from - 1, to - 1 //Correct the input parse with slices initial positions
}

func main() {
	part1()
	part2()
}
