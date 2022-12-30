package main

import (
	"bufio"
	"fmt"
	"log"
	"math"
	"os"
)

var fuels []string

var conversion = map[byte]int{ // SNAFU uses 5 possible characters as well as base-5, but not all are numbers
	'2': 2,
	'1': 1,
	'0': 0,
	'-': -1,
	'=': -2,
}

var conversionFromDec = map[int]byte{ // This is the same as above but changing the keys/values, since there is no builtin method in Go
	0: '0',
	1: '1',
	2: '2',
	3: '=',
	4: '-',
}

func part1() {
	var total int
	for _, fuel := range fuels {
		total += decode(fuel)
	}

	snafuNumber := encode(total)

	fmt.Printf("Part 1 solution: %s\n", snafuNumber)
}

// Basically, this is a base-5 system with some twists
// Loop up for how to decode from base-5 to base-10 for a better explanation
func decode(line string) int {
	var decimal int
	var power float64

	for i := len(line) - 1; i > -1; i-- { // Since to convert bases we need to start from the end of the number, we need to loop backwards
		decimal += conversion[line[i]] * int(math.Pow(5, power)) // Standard process of converting to base-5, the only differences is that we get the values from the conversion map
		power++
	}

	return decimal
}

// The other functions converts from SNAFU to decimal, so this one is the other way around
// To understand this, basically, you need to look up on how to convert from base-10 to base-5
// A Google search would be better at explaining that than myself
func encode(decimal int) string {
	var snafu string

	for decimal > 0 {
		remainder := decimal % 5
		digit := conversionFromDec[remainder]
		if digit == '-' || digit == '=' {
			decimal += remainder // Adjust, since those value are "negative" in the real SNAFU. That's why in the map are 3 and 4
		}
		snafu = string(digit) + snafu
		decimal /= 5
	}

	return snafu
}

func parseFuel() {
	file, err := os.Open("input.txt")
	if err != nil {
		log.Fatal(err)
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		fuels = append(fuels, scanner.Text())
	}
}

func main() {
	parseFuel()
	part1()
}
