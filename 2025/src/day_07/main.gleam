import gleam/dict
import gleam/format
import gleam/list
import gleam/set.{type Set}
import gleam/string
import simplifile

pub fn main() {
  let assert Ok(content) = simplifile.read(from: "./src/day_07/input.txt")
  // Coopy the same parsing grid from day 4.
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
}

// I find this part pretty straightforward: save the starting point, save a set of where
// the splitters are and keep a set of where the current beams are.
// Use recursion (perfect and easy for Gleam as far as I've seen) to keep on iterating and counting
// until the set of beams is empty!
fn part_1(grid: dict.Dict(#(Int, Int), String)) -> Int {
  // Find starting point and splitters in one pass
  let #(start, splitters) =
    dict.fold(grid, from: #(Error(Nil), set.new()), with: fn(acc, pos, char) {
      let #(start_acc, splitters_acc) = acc
      case char {
        "S" -> #(Ok(pos), splitters_acc)
        "^" -> #(start_acc, set.insert(splitters_acc, pos))
        _ -> acc
      }
    })
  let assert Ok(start_pos) = start

  let initial_beams = set.from_list([start_pos])
  let hit_splitters =
    count_unique_splitters(initial_beams, splitters, grid, set.new())
  set.size(hit_splitters)
}

fn count_unique_splitters(
  beams: Set(#(Int, Int)),
  splitters: Set(#(Int, Int)),
  grid: dict.Dict(#(Int, Int), String),
  hit_splitters: Set(#(Int, Int)),
) -> Set(#(Int, Int)) {
  case set.size(beams) {
    0 -> hit_splitters
    _ -> {
      let new_state =
        set.fold(beams, from: #(set.new(), hit_splitters), with: fn(acc, beam) {
          let #(new_beams, current_hits) = acc
          let #(row, col) = beam
          let next_pos = #(row + 1, col)
          case dict.get(grid, next_pos) {
            Error(_) -> #(new_beams, current_hits)
            Ok(_) -> {
              case set.contains(splitters, next_pos) {
                True -> {
                  // Hit splitter: remove current beam, add two new ones
                  let left = #(row + 2, col - 1)
                  let right = #(row + 2, col + 1)
                  #(
                    new_beams |> set.insert(left) |> set.insert(right),
                    set.insert(current_hits, next_pos),
                  )
                }
                False -> {
                  // Continue: move beam to next position
                  #(set.insert(new_beams, next_pos), current_hits)
                }
              }
            }
          }
        })
      let #(next_beams, updated_hits) = new_state
      count_unique_splitters(next_beams, splitters, grid, updated_hits)
    }
  }
}
