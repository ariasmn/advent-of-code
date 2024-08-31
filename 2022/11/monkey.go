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

var monkeys []*Monkey
var inspected []int

type Monkey struct {
	items         []int64
	operator      byte
	firstOperand  string
	secondOperand string
	testDiv       int64
	trueTo        int64
	falseTo       int64
	inspected     int
}

func (monkey *Monkey) ParseItems(itemsLine string) {
	_, itemsLine, _ = strings.Cut(itemsLine, " Starting items: ") // itemsLine now holds items separated by space and comma
	items := strings.Split(itemsLine, ", ")                       // Items inside a slice, string format

	for _, item := range items {
		parsedItem, _ := strconv.ParseInt(item, 10, 64)
		monkey.items = append(monkey.items, parsedItem)
	}
}

func (monkey *Monkey) ParseOperation(operationLine string) {
	fmt.Sscanf(operationLine, "Operation: new = %s %c %s", &monkey.firstOperand, &monkey.operator, &monkey.secondOperand)
}

func (monkey *Monkey) ParseTestDiv(testDivLine string) {
	fmt.Sscanf(testDivLine, "Test: divisible by %d", &monkey.testDiv)
}

func (monkey *Monkey) ParseIfTrue(trueToLine string) {
	fmt.Sscanf(trueToLine, "If true: throw to monkey %d", &monkey.trueTo)
}

func (monkey *Monkey) ParseIfFalse(trueToLine string) {
	fmt.Sscanf(trueToLine, "If false: throw to monkey %d", &monkey.falseTo)
}

func (monkey *Monkey) GetNewItemWorryLevel(itemValue int64) int64 {
	var trueFirstOperand, trueSecondOperand int64

	if monkey.firstOperand == "old" {
		trueFirstOperand = itemValue
	} else {
		trueFirstOperand, _ = strconv.ParseInt(monkey.firstOperand, 10, 64)
	}

	if monkey.secondOperand == "old" {
		trueSecondOperand = itemValue
	} else {
		trueSecondOperand, _ = strconv.ParseInt(monkey.secondOperand, 10, 64)
	}

	// Seems like only addition or multiplication are the only possibles operations
	if monkey.operator == '*' {
		return (trueFirstOperand * trueSecondOperand)
	}

	return (trueFirstOperand + trueSecondOperand)
}

func part1() {
	parseMonkeys()

	for round := 0; round < 20; round++ {
		for _, monkey := range monkeys {
			for _, item := range monkey.items {
				itemWorryLevel := monkey.GetNewItemWorryLevel(item) / 3
				if itemWorryLevel%monkey.testDiv == 0 {
					monkeys[monkey.trueTo].items = append(monkeys[monkey.trueTo].items, itemWorryLevel)
				} else {
					monkeys[monkey.falseTo].items = append(monkeys[monkey.falseTo].items, itemWorryLevel)
				}
				monkey.inspected++
			}
			monkey.items = []int64{} // After we completed the monkey's round, we empty all the items since those are already passed to other monkeys
		}
	}

	fmt.Printf("Part 1 solution :%d\n", getMonkeyBussiness())
}

func part2() {
	parseMonkeys()

	var bigMod int64 = 1

	// We calculate the least common multiple
	// Since all numbers are prime, the result is the product of all the test divisible numbers
	for _, monkey := range monkeys {
		bigMod *= monkey.testDiv
	}

	for round := 0; round < 10000; round++ {
		for _, monkey := range monkeys {
			for _, item := range monkey.items {
				itemWorryLevel := monkey.GetNewItemWorryLevel(item) % bigMod // Using modular arithmetic to solve the problem
				if itemWorryLevel%monkey.testDiv == 0 {
					monkeys[monkey.trueTo].items = append(monkeys[monkey.trueTo].items, itemWorryLevel)
				} else {
					monkeys[monkey.falseTo].items = append(monkeys[monkey.falseTo].items, itemWorryLevel)
				}
				monkey.inspected++
			}
			monkey.items = []int64{} // After we completed the monkey's round, we empty all the items since those are already passed to other monkeys
		}
	}

	fmt.Printf("Part 2 solution :%d\n", getMonkeyBussiness())
}

func parseMonkeys() {
	file, err := os.Open("input.txt")
	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)

	monkeys = []*Monkey{}

	for scanner.Scan() {
		if scanner.Text() == "" { // Skip separation line between monkeys
			scanner.Scan()
		}
		scanner.Scan() // Skip first line of each monkey, which indicates the index
		var newMonkey Monkey

		itemsLine := scanner.Text()
		newMonkey.ParseItems(itemsLine)

		scanner.Scan()
		operationLine := scanner.Text()
		newMonkey.ParseOperation(strings.TrimSpace(operationLine))

		scanner.Scan()
		testDivLine := scanner.Text()
		newMonkey.ParseTestDiv(strings.TrimSpace(testDivLine))

		scanner.Scan()
		trueToLine := scanner.Text()
		newMonkey.ParseIfTrue(strings.TrimSpace(trueToLine))

		scanner.Scan()
		falseToLine := scanner.Text()
		newMonkey.ParseIfFalse(strings.TrimSpace(falseToLine))

		monkeys = append(monkeys, &newMonkey)
	}
}

func getMonkeyBussiness() int {
	inspects := []int{}
	for _, monkey := range monkeys {
		inspects = append(inspects, monkey.inspected)
	}
	sort.Sort(sort.Reverse(sort.IntSlice(inspects)))

	return inspects[0] * inspects[1]
}

func main() {
	part1()
	part2()
}
