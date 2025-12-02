import gleam/format
import gleam/int
import gleam/list
import gleam/string
import simplifile

pub fn main() {
  let assert Ok(content) = simplifile.read(from: "./src/day_02/input.txt")
  let ids = string.split(content, ",")

  format.printf("Part 1: ~b\n", part_1(ids))
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

      // Range over the start and end
      let range_sum =
        list.range(start, end)
        // Filter out those numbers that are not invalid (not repeated)
        |> list.filter(fn(num) {
          // Get the number as string so we can check length/split
          let num_str = int.to_string(num)
          let num_len = string.length(num_str)
          case num_len % 2 {
            // We only care if the length is even, since if it would be impossible
            // for odd numbers to comply this rule.
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
