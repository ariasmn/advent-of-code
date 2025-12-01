// src/day_01/main.gleam
import gleam/format
import gleam/int
import gleam/list
import gleam/string
import simplifile

pub fn main() {
  // assert is similar to panic in Go. If the file can't be read, it will panic.
  let assert Ok(content) = simplifile.read(from: "./src/day_01/input.txt")
  // we store the lines here in a list we can range.
  let lines = string.split(content, "\n")
  format.printf("Part 1: ~b\n", part_1(lines))
}

fn part_1(lines: List(String)) -> Int {
  // The '#(a, b)' syntax is the way we declare tuples.
  // Ideally we would use a new type for this maybe? but it's cool to learn this.
  let starting_point = #(50, 0)
  // Same as Go, we can use '_' if we don't want to use the value.
  let #(_, count) =
    lines
    // Remove empty lines BEFORE fold
    |> list.filter(fn(line) { line != "" })
    |> list.fold(from: starting_point, with: fn(acc, line) {
      let #(current_pos, zero_count) = acc

      // Calculate the new position using the rotate function
      let new_pos = rotate(current_pos, line)
      // If it's 0, we need to increase the count which is the solution. If not, do nothing.
      let new_count = case new_pos {
        0 -> zero_count + 1
        _ -> zero_count
      }

      #(new_pos, new_count)
    })
  count
}

// rotate calculates the rotation taking into account the range is 0-100,
// and return the new position.
fn rotate(current: Int, instruction: String) -> Int {
  let assert Ok(direction) = string.first(instruction)
  let assert Ok(distance) =
    string.drop_start(instruction, 1)
    |> int.parse()

  let new_pos = case direction {
    "L" -> current - distance
    "R" -> current + distance
    _ -> panic as "Invalid direction"
  }

  { new_pos % 100 }
}
