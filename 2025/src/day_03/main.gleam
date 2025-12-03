import gleam/format
import gleam/int
import gleam/list
import gleam/string
import simplifile

pub fn main() {
  let assert Ok(content) = simplifile.read(from: "./src/day_03/input.txt")
  let lines = string.split(content, "\n")

  format.printf("Part 1: ~b\n", part_1(lines))
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
