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

var monkeys = map[string]string{} // This map holds each monkey and its operation. We could use a new struct but it's easier this way to use recursion IMO

func part1() {
	fmt.Printf("Part 1 solution: %d\n", calculate("root"))
}

// Since "humn" is only used in one branch of the two, one of the numbers is static
// Knowing that, first thing that came to mind was bruteforcing using divide and conquer (since backtracking to solve equations is too much)
// Actually, Go's sort.Find let you do exactly that! (kinda)
func part2() {
	monkeys["humn"] = "0"                                          // We set humn to 0 for the posible calculations
	rootOperators := strings.Fields(monkeys["root"])               // We get both operators of root
	if calculate(rootOperators[0]) < calculate(rootOperators[2]) { // We need to keep track of what number is lower, to do the "sort.Find" correctly
		rootOperators[0], rootOperators[2] = rootOperators[2], rootOperators[0] // In case 0's number is lower, we switch it
	}

	// Basically, sort.Find executes until the return callback value is <=0, which is exactly what we want (both numbers to be equal, so 0)
	// Inside, it actually uses binary search using divide and conquer
	// For more information, read the documentation: https://pkg.go.dev/sort#Find
	equality, _ := sort.Find(10_000_000_000_000, func(possibleValue int) int {
		monkeys["humn"] = strconv.Itoa(possibleValue)                    // We parse from int to string, since the caclulate function uses a string and parses to int in there
		return calculate(rootOperators[0]) - calculate(rootOperators[2]) // We calculate both numbers until it returns 0, so they are equal
	})

	fmt.Printf("Part 2 solution: %d\n", equality)
}

// This is a clear recursion problem. We just call the function until it returns a number, to calculate the root operation.
func calculate(monkeyName string) int {
	inputs := strings.Fields(monkeys[monkeyName]) // Slice with operators and operand
	if len(inputs) == 1 {                         // No operation, just a number, so we return it
		num, _ := strconv.Atoi(inputs[0])
		return num
	}

	operator := inputs[1] // We get the type of operation to execute and use recursion
	// Should be a cleaner way to do this
	if operator == "+" {
		return calculate(inputs[0]) + calculate(inputs[2])
	}
	if operator == "-" {
		return calculate(inputs[0]) - calculate(inputs[2])
	}
	if operator == "*" {
		return calculate(inputs[0]) * calculate(inputs[2])
	}
	if operator == "/" {
		return calculate(inputs[0]) / calculate(inputs[2])
	}

	panic("Unknown operator")
}

func parseMonkeys() {
	file, err := os.Open("input.txt")
	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()
		splittedLine := strings.Split(line, ": ")  // Now we have a string slice containing the monkey and its operation
		monkeys[splittedLine[0]] = splittedLine[1] // Store it into the map
	}
}

func main() {
	parseMonkeys()
	part1()
	part2()
}
