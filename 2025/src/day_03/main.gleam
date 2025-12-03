import gleam/format
import gleam/int
import gleam/list
import gleam/string
import simplifile

pub fn main() {
  let assert Ok(content) = simplifile.read(from: "./src/day_03/input.txt")
  let lines = string.split(content, "\n")

  format.printf("Part 1: ~b\n", part_1(lines))
  format.printf("Part 2: ~b\n", part_2(lines))
}

fn part_1(lines: List(String)) -> Int {
  lines
  |> list.filter(fn(line) { line != "" })
  |> list.fold(from: 0, with: fn(acc, bank_line) {
    let graphemes = string.to_graphemes(bank_line)
    let graphemes_length = list.length(graphemes)

    let #(first_max, second_max) =
      graphemes
      |> list.index_fold(from: #(0, 0), with: fn(bats, bank, index) {
        let assert Ok(value) = int.parse(bank)
        let #(max1, max2) = bats
        let is_last = index == graphemes_length - 1

        // If it's the last element, we can't set a higher first number
        // so we only need to check the second one.
        case is_last {
          True ->
            case value > max2 {
              True -> #(max1, value)
              False -> bats
            }
          False -> {
            // Not last element - normal logic
            case value > max1, value > max2 {
              True, _ -> #(value, 0)
              False, True -> #(max1, value)
              False, False -> bats
            }
          }
        }
      })

    acc + { first_max * 10 + second_max }
  })
}

// Doing this was quite hard even in Go, so using Gleam (and I'm not only new to Gleam, but also FP)
// was even harder. This post in Reddit really helped me visualize it and implement it.
// https://www.reddit.com/r/adventofcode/comments/1pd0cp6/2025_day_03_cli_visualization/
fn part_2(lines: List(String)) -> Int {
  lines
  |> list.filter(fn(line) { line != "" })
  |> list.fold(from: 0, with: fn(acc, bank_line) {
    let graphemes = string.to_graphemes(bank_line)
    let graphemes_length = list.length(graphemes)
    // length of the number we are trying to build
    let target_count = 12

    // Build the largest 12-digit number by greedily selecting digits
    // At each position, take the current digit unless we can afford to skip it
    // (have digits remaining to fill) AND a larger digit exists ahead in the window.
    // This is kind of like a sliding window, but the window length is not fixed.
    let result =
      graphemes
      |> list.index_fold(from: #([], 0), with: fn(state, digit, index) {
        let #(selected, selected_count) = state
        case selected_count == target_count {
          // If we've already selected 12 digits, we're done.
          True -> state
          False -> {
            // Digits we still need (window length).
            let remaining_needed = target_count - selected_count
            // Digits available after this position.
            let remaining_available = graphemes_length - index
            // We can skip if we have more digits available than we need.
            let can_skip = remaining_available > remaining_needed

            case can_skip {
              True -> {
                // Look ahead to see if there's a better digit we can use.
                let assert Ok(current_value) = int.parse(digit)
                let has_better_ahead =
                  graphemes
                  |> list.drop(index + 1)
                  |> list.take(remaining_available - remaining_needed)
                  |> list.any(fn(future_digit) {
                    let assert Ok(future_value) = int.parse(future_digit)
                    future_value > current_value
                  })

                case has_better_ahead {
                  // Skip this digit
                  True -> state
                  False -> #(list.append(selected, [digit]), selected_count + 1)
                }
              }
              False -> {
                // Must take this digit since we need all the remaining.
                #(list.append(selected, [digit]), selected_count + 1)
              }
            }
          }
        }
      })

    let #(selected_digits, _) = result
    let number_string = string.concat(selected_digits)
    let assert Ok(max_number) = int.parse(number_string)

    acc + max_number
  })
}
