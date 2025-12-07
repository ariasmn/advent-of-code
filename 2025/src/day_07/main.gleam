import gleam/dict
import gleam/format
import gleam/list
import gleam/set
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
  format.printf("Part 2: ~b\n", part_2(grid))
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

// I honestly don't know if there's a better way of doing this, but as always, when implementing something that works
// for the test input but takes super long for my input in this kind of overlapping problems,
// my approach is to use DP and use memoization, which is what I did here.
// It works and it's fast so no complaints I guess.
fn part_2(grid: dict.Dict(#(Int, Int), String)) -> Int {
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

  // Count paths using memoization
  count_paths(start_pos, splitters, grid, dict.new())
  |> fn(result) { result.0 }
}

fn count_unique_splitters(
  beams: set.Set(#(Int, Int)),
  splitters: set.Set(#(Int, Int)),
  grid: dict.Dict(#(Int, Int), String),
  hit_splitters: set.Set(#(Int, Int)),
) -> set.Set(#(Int, Int)) {
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

fn count_paths(
  pos: #(Int, Int),
  splitters: set.Set(#(Int, Int)),
  grid: dict.Dict(#(Int, Int), String),
  memo: dict.Dict(#(Int, Int), Int),
) -> #(Int, dict.Dict(#(Int, Int), Int)) {
  // Check memo
  case dict.get(memo, pos) {
    Ok(cached) -> #(cached, memo)
    Error(_) -> {
      let #(row, col) = pos
      let next_pos = #(row + 1, col)

      case dict.get(grid, next_pos) {
        Error(_) -> {
          // Out of bounds = 1 timeline ends here
          #(1, dict.insert(memo, pos, 1))
        }
        Ok(_) -> {
          case set.contains(splitters, next_pos) {
            True -> {
              // Split: count paths from both branches
              let left = #(row + 2, col - 1)
              let right = #(row + 2, col + 1)

              let #(left_count, memo1) =
                count_paths(left, splitters, grid, memo)
              let #(right_count, memo2) =
                count_paths(right, splitters, grid, memo1)

              let total = left_count + right_count
              #(total, dict.insert(memo2, pos, total))
            }
            False -> {
              // Continue straight
              let #(next_count, memo1) =
                count_paths(next_pos, splitters, grid, memo)
              #(next_count, dict.insert(memo1, pos, next_count))
            }
          }
        }
      }
    }
  }
}
