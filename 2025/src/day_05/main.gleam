import gleam/format
import gleam/int
import gleam/list
import gleam/string
import simplifile

pub fn main() {
  let assert Ok(content) = simplifile.read(from: "./src/day_05/input.txt")

  format.printf("Part 1: ~b\n", part_1(content))
  format.printf("Part 2: ~b\n", part_2(content))
}

fn part_1(lines: String) -> Int {
  // We get two sections here: one with the ranges, and after two new lines, the list of ingredientes.
  let assert [ranges_sec, ingredients_sec] = string.split(lines, "\n\n")

  let ranges =
    string.split(ranges_sec, "\n")
    |> list.map(fn(line) {
      let assert Ok(#(start_str, end_str)) = string.split_once(line, on: "-")
      let assert Ok(start) = int.parse(start_str)
      let assert Ok(end) = int.parse(end_str)
      #(start, end)
    })

  let ingredients =
    string.split(ingredients_sec, "\n")
    // Remove the line that split the sections. I don't know if there's a native way of doing this,
    // but haven't found it.
    |> list.filter(fn(line) { line != "" })
    |> list.map(fn(line) {
      let assert Ok(ing) = int.parse(line)
      ing
    })

  // Now we have two lists: one with the ranges and one with the ingredients.
  // A bit inefficient, but let's iterate the ingredients, and then for each one,
  // iterate over all the ranges and check if the ID is in any of the ranges.
  let found =
    ingredients
    |> list.fold(from: 0, with: fn(acc, ing) {
      let in_range =
        ranges
        |> list.any(fn(range) {
          let #(start, end) = range
          ing >= start && ing <= end
        })
      case in_range {
        True -> acc + 1
        False -> acc
      }
    })
  found
}

fn part_2(lines: String) -> Int {
  // We get two sections again but only care for the first one.
  let assert [ranges_sec, _] = string.split(lines, "\n\n")

  let ranges =
    string.split(ranges_sec, "\n")
    |> list.map(fn(line) {
      let assert Ok(#(start_str, end_str)) = string.split_once(line, on: "-")
      let assert Ok(start) = int.parse(start_str)
      let assert Ok(end) = int.parse(end_str)
      #(start, end)
    })

  // Bruteforcing here is not an option. Luckily, this is a case of overlapping ranges,
  // which I think I had to do years ago in another AoC.
  //
  // Sort ranges by start position.
  let sorted_ranges =
    list.sort(ranges, fn(a, b) {
      let #(start_a, _) = a
      let #(start_b, _) = b
      int.compare(start_a, start_b)
    })

  // Merge overlapping ranges.
  let clean_ranges_overlapped =
    list.fold(sorted_ranges, from: [], with: fn(merged, current) {
      let #(curr_start, curr_end) = current
      case merged {
        // First range, just add it.
        [] -> [current]
        // We have existing merged ranges: check if current overlaps with the last one.
        // 'previous' is the result of the destructuring, meaning, the rest of tuples already merged.
        [last, ..previous] -> {
          let #(last_start, last_end) = last
          // Check if ranges overlap or touch.
          case curr_start <= last_end + 1 {
            True -> [#(last_start, int.max(last_end, curr_end)), ..previous]
            False -> [current, ..merged]
          }
        }
      }
    })

  list.fold(clean_ranges_overlapped, from: 0, with: fn(acc, range) {
    let #(start, end) = range
    acc + { end - start + 1 }
  })
}
