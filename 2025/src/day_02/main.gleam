import gleam/format
import gleam/int
import gleam/list
import gleam/string
import simplifile

pub fn main() {
  let assert Ok(content) = simplifile.read(from: "./src/day_02/input.txt")
  let ids = string.split(content, ",")

  // Yes, even part 2 made without regex
  // Super slow but you couldn't force me to use regex unless strictly necessary!
  format.printf("Part 1: ~b\n", part_1(ids))
  format.printf("Part 2: ~b\n", part_2(ids))
}

fn part_1(ids: List(String)) -> Int {
  let total_sum =
    ids
    |> list.fold(from: 0, with: fn(acc, id_text) {
      // Trim new lines, for the last value. We could remove the new line in the file but whatever.
      let id_text = string.trim(id_text)
      // Get the start and end of the range. Panic if we can't split by "-".
      let assert Ok(#(start_str, end_str)) = string.split_once(id_text, on: "-")

      // Parse the strings to int so we can range
      let assert Ok(start) = int.parse(start_str)
      let assert Ok(end) = int.parse(end_str)

      // Range over the start and end.
      let range_sum =
        list.range(start, end)
        |> list.filter(fn(num) {
          // Get the number as string so we can check length/split.
          let num_str = int.to_string(num)
          let num_len = string.length(num_str)
          case num_len % 2 {
            // We only care if the length is even, since if it would be impossible
            // for odd numbers to comply this rule of repeating.
            0 -> {
              let half = num_len / 2
              let first_half = string.slice(num_str, 0, half)
              let second_half = string.slice(num_str, half, half)

              first_half == second_half
            }
            _ -> False
          }
        })
        |> int.sum()
      acc + range_sum
    })

  echo total_sum
}

fn part_2(ids: List(String)) -> Int {
  ids
  |> list.fold(from: 0, with: fn(acc, id_text) {
    let id_text = string.trim(id_text)
    let assert Ok(#(start_str, end_str)) = string.split_once(id_text, on: "-")
    let assert Ok(start) = int.parse(start_str)
    let assert Ok(end) = int.parse(end_str)

    let range_sum =
      list.range(start, end)
      // Filter out those numbers that have a repeating pattern.
      |> list.filter(fn(num) {
        let num_str = int.to_string(num)
        let length = string.length(num_str)

        // Try all possible pattern lengths from 1 to half the total length.
        // Example: for "123456" (length 6), try patterns of length 1, 2, 3
        // We go to length/2 because a pattern needs to repeat at least twice.
        list.range(1, length / 2)
        // This broke my mind: this is actually a way of doing just a for loop.
        // FP is damn hard.
        |> list.any(fn(pattern_length) {
          // If it can be divided evenly by the pattern length we should check.
          case length % pattern_length {
            0 -> {
              let pattern = string.slice(num_str, 0, pattern_length)
              let repetitions = length / pattern_length
              // Pattern must repeat at least twice.
              case repetitions >= 2 {
                True -> {
                  // Check if the repeated number matches the original number.
                  // Example: "123123" was the original number, but we have "123" in this pattern.
                  string.repeat(pattern, repetitions) == num_str
                }
                False -> False
              }
            }
            _ -> False
          }
        })
      })
      |> int.sum()
    acc + range_sum
  })
}
