// src/day_01/main.gleam
import gleam/format
import gleam/int
import gleam/list
import gleam/result
import gleam/string
import simplifile

pub fn main() {
  // assert is similar to panic in Go. If the file can't be read, it will panic.
  let assert Ok(content) = simplifile.read(from: "./src/day_01/input.txt")
  // we store the lines here in a list we can range.
  let lines = string.split(content, "\n")

  format.printf("Part 1: ~b\n", part_1(lines))
  format.printf("Part 2: ~b\n", part_2(lines))
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
      let #(new_pos, _) = rotate_and_count(current_pos, line)
      // If it's 0, we need to increase the count which is the solution. If not, do nothing.
      let new_count = case new_pos {
        0 -> zero_count + 1
        _ -> zero_count
      }

      #(new_pos, new_count)
    })
  count
}

fn part_2(lines: List(String)) -> Int {
  let starting_point = #(50, 0)
  let #(_, count) =
    lines
    |> list.filter(fn(line) { line != "" })
    |> list.fold(from: starting_point, with: fn(acc, line) {
      let #(current_pos, zero_count) = acc
      let #(new_pos, passes) = rotate_and_count(current_pos, line)

      #(new_pos, zero_count + passes)
    })

  count
}

// rotate_and_count calculates the rotation taking into account the range is 0-100,
// and return the new position as well as how many times it's passed through 0 in this iteration.
fn rotate_and_count(current: Int, instruction: String) -> #(Int, Int) {
  let assert Ok(direction) = string.first(instruction)
  let assert Ok(clicks) =
    string.drop_start(instruction, 1)
    |> int.parse()

  let #(new_pos_raw, passes) = case direction {
    "L" -> {
      // Count how many clicks we need to take to get to 0.
      let clicks_to_zero = case current {
        // Already at 0, next is after 100 clicks.
        0 -> 100
        // Hit 0 after 'current' clicks (because we're going backwards).
        _ -> current
      }
      // We can calculate knowing if the number of clicks we are going to do will take us to 0.
      let p = case clicks >= clicks_to_zero {
        True -> 1 + { clicks - clicks_to_zero } / 100
        False -> 0
      }
      #(current - clicks, p)
    }
    "R" -> {
      // For this case, since it's positive, the math is simple to calculate how many times it has passed 0.
      let p = { current + clicks } / 100
      #(current + clicks, p)
    }
    _ -> panic as "Invalid direction"
  }

  // Why try to do modular arithmetic when a way more intelligent person than me
  // already did it and added it to the standard lib? :D
  // Unwrap returns the value if there's an error. It's impossible to get an error because the divisor is always 100,
  // and this only returns an error if it's 0, but we still need to do something about it.
  let new_pos = int.modulo(new_pos_raw, 100) |> result.unwrap(0)

  #(new_pos, passes)
}
