import gleam/format
import gleam/int
import gleam/list
import gleam/string
import simplifile

pub fn main() {
  let assert Ok(content) = simplifile.read(from: "./src/day_09/input.txt")

  let positions =
    content
    |> string.split("\n")
    |> list.filter(fn(line) { line != "" })
    |> list.map(fn(line) {
      let assert [x_str, y_str] = string.split(line, ",")
      let assert Ok(x) = int.parse(x_str)
      let assert Ok(y) = int.parse(y_str)
      #(x, y)
    })

  format.printf("Part 1: ~b\n", part_1(positions))
  format.printf("Part 2: ~b\n", part_2(positions))
}

// Using the function I discovered in the last day, this part is too easy:
// generate a list of pair for all the points, and calculate the area for all of them.
// The result is the hightest area calculated. The input is not that big, so it's quite fast.
fn part_1(positions: List(#(Int, Int))) -> Int {
  list.combination_pairs(positions)
  |> list.map(fn(pair) {
    let #(p1, p2) = pair
    let #(x1, y1) = p1
    let #(x2, y2) = p2

    // Calculate width and height (adding 1 to include both endpoints)
    let width = int.absolute_value(x2 - x1) + 1
    let height = int.absolute_value(y2 - y1) + 1

    width * height
  })
  |> list.fold(from: 0, with: int.max)
}

fn part_2(_: List(#(Int, Int))) -> Int {
  0
}
