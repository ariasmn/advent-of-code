import gleam/dict
import gleam/format
import gleam/list
import gleam/string
import simplifile

pub fn main() {
  let assert Ok(content) = simplifile.read(from: "./src/day_04/input.txt")
  // Parse the content into a grid. The key is a tuple with the coords.
  let grid =
    content
    |> string.split("\n")
    |> list.filter(fn(line) { line != "" })
    |> list.index_fold(from: dict.new(), with: fn(grid, line, row) {
      string.to_graphemes(line)
      |> list.index_fold(from: grid, with: fn(g, char, col) {
        dict.insert(g, #(row, col), char)
      })
    })

  format.printf("Part 1: ~b\n", part_1(grid))
  format.printf("Part 2: ~b\n", part_2(grid))
}

fn part_1(grid: dict.Dict(#(Int, Int), String)) -> Int {
  let rolls =
    dict.fold(grid, from: 0, with: fn(acc, pos, val) {
      // We only need to check the neighbours when there's a roll.
      case val == "@" {
        False -> acc
        True -> {
          let #(row, col) = pos
          // Get all 8 neighbours
          let neighbours =
            [
              #(row - 1, col - 1),
              #(row - 1, col),
              #(row - 1, col + 1),
              #(row, col - 1),
              #(row, col + 1),
              #(row + 1, col - 1),
              #(row + 1, col),
              #(row + 1, col + 1),
            ]
            |> list.filter_map(fn(neighbor_pos) { dict.get(grid, neighbor_pos) })

          // Count how many neighbours.
          let neighbour_count =
            neighbours
            |> list.count(fn(n) { n == "@" })

          // format.printf("Pos ~b-~b, neighbours: ~b\n", [
          //   row,
          //   col,
          //   neighbour_count,
          // ])

          case neighbour_count < 4 {
            True -> acc + 1
            False -> acc
          }
        }
      }
    })
  rolls
}

fn part_2(_: dict.Dict(#(Int, Int), String)) -> Int {
  0
}
