import gleam/format
import gleam/int
import gleam/list
import gleam/string
import simplifile

pub fn main() {
  let assert Ok(content) = simplifile.read(from: "./src/day_05/input.txt")

  format.printf("Part 1: ~b\n", part_1(content))
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
