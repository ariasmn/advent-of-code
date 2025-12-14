import gleam/format
import gleam/int
import gleam/list
import gleam/string
import simplifile

pub type Rectangle {
  Rectangle(min_x: Int, max_x: Int, min_y: Int, max_y: Int, area: Int)
}

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

// I really struggled with this. I tried to use Ray Casting, as I learnt in 2023 day 10, to delimit each rectangle,
// but this was quite slow since we need to check all the points, so I did manage to get one solution that worked for
// the test input, but not for the normal input.
// I found some help in this blog https://todd.ginsberg.com/post/advent-of-code/2025/day9/#d9p2, which explained
// his approach and it really clicked. It's actually quite simple once you understand it to be honest.
fn part_2(positions: List(#(Int, Int))) -> Int {
  // Create all rectangles from position pairs, sorted by area (largest first).
  let rectangles =
    list.combination_pairs(positions)
    |> list.map(fn(pair) {
      let #(#(x1, y1), #(x2, y2)) = pair
      let min_x = int.min(x1, x2)
      let max_x = int.max(x1, x2)
      let min_y = int.min(y1, y2)
      let max_y = int.max(y1, y2)
      let area = { max_x - min_x + 1 } * { max_y - min_y + 1 }
      Rectangle(min_x, max_x, min_y, max_y, area)
    })
    |> list.sort(fn(a, b) { int.compare(b.area, a.area) })

  // Create boundary lines between consecutive red tiles.
  let assert Ok(first_pos) = list.first(positions)
  let lines =
    list.append(positions, [first_pos])
    |> list.window_by_2()
    |> list.map(fn(pair) {
      let #(#(x1, y1), #(x2, y2)) = pair
      Rectangle(
        int.min(x1, x2),
        int.max(x1, x2),
        int.min(y1, y2),
        int.max(y1, y2),
        0,
      )
    })

  // For each rectangle (starting with largest):
  // Shrink it by 1 on all sides to get the "inner rectangle"
  // Check if this inner rectangle overlaps ANY boundary line.
  // If NO overlap, the rectangle is valid.
  // Return its area, which should be the solution since it's sorted.
  let assert Ok(valid_rect) =
    list.find(rectangles, fn(rect) {
      // Inner rectangle (shrink by 1 on all sides)
      let inner =
        Rectangle(
          rect.min_x + 1,
          rect.max_x - 1,
          rect.min_y + 1,
          rect.max_y - 1,
          0,
        )

      // Check if any line overlaps the inner rectangle
      !list.any(lines, fn(line) {
        ranges_overlap(inner.min_x, inner.max_x, line.min_x, line.max_x)
        && ranges_overlap(inner.min_y, inner.max_y, line.min_y, line.max_y)
      })
    })

  valid_rect.area
}

// Check if two ranges overlap
fn ranges_overlap(a_min: Int, a_max: Int, b_min: Int, b_max: Int) -> Bool {
  int.max(a_min, b_min) <= int.min(a_max, b_max)
}
