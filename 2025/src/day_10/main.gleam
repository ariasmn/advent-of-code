import gleam/dict
import gleam/format
import gleam/int
import gleam/io
import gleam/list
import gleam/set
import gleam/string
import simplifile

type Machine {
  Machine(target: List(Bool), buttons: List(List(Int)), joltage: List(Int))
}

pub fn main() {
  let assert Ok(content) = simplifile.read(from: "./src/day_10/input.txt")
  let machines =
    content
    |> string.split("\n")
    |> list.filter(fn(line) { line != "" })
    |> list.map(fn(line) {
      // Split by space: first part is target [.##.], rest is buttons
      let parts = string.split(line, " ")
      let assert Ok(target_part) = list.first(parts)
      let assert Ok(joltage_part) = list.last(parts)

      let buttons_parts =
        parts
        |> list.drop(1)
        |> list.take(list.length(parts) - 2)

      // Filter the "[]" chars and then map the targets that are "#",
      // which are the lights that we need to turn on, thus those that need to be true.
      let target =
        string.to_graphemes(target_part)
        |> list.filter(fn(c) { c != "[" && c != "]" })
        |> list.map(fn(c) { c == "#" })

      // Remove the "()" from the string and then split by the comma and parse the integers.
      let buttons =
        list.map(buttons_parts, fn(button_str) {
          string.replace(button_str, "(", "")
          |> string.replace(")", "")
          |> string.split(",")
          |> list.map(fn(n) {
            let assert Ok(num) = int.parse(n)
            num
          })
        })

      // Remove the "{}" from the string and then split by the comma and parse the integers.
      let joltage =
        string.replace(joltage_part, "{", "")
        |> string.replace("}", "")
        |> string.split(",")
        |> list.map(fn(n) {
          let assert Ok(num) = int.parse(n)
          num
        })

      Machine(target, buttons, joltage)
    })

  format.printf("Part 1: ~b\n", part_1(machines))
  format.printf("Part 2: ~b\n", part_2(machines))
}

// I solved this by using good old BFS (this feels like home in an AoC context :D).
// BFS is the correct approach here, not DFS, because we need to find the minimum number of buttons pressed.
// This will probably be trash for part 2, but whatever.
fn part_1(machines: List(Machine)) -> Int {
  machines
  |> list.map(fn(machine) {
    // For each machine, the inisial state is just a list of the lights turned off.
    let initial_state = list.repeat(False, list.length(machine.target))
    case bfs([#(initial_state, 0)], set.new(), machine) {
      Ok(presses) -> presses
      Error(msg) -> {
        io.println(msg)
        panic as "BFS failed to find solution"
      }
    }
  })
  |> list.fold(0, int.add)
}

// BFS, not much to explain here!
fn bfs(
  queue: List(#(List(Bool), Int)),
  visited: set.Set(List(Bool)),
  machine: Machine,
) -> Result(Int, String) {
  case queue {
    // No solution found
    [] -> Error("Could not find solution. Maybe input is not parsed correctly?")
    // This is the "rest pattern", which basically takes the first one of the list, destructure into a tuple, and
    // leave the rest of the list in the "rest" variable.
    // https://tour.gleam.run/everything/#flow-control-list-recursion
    [#(state, presses), ..rest] -> {
      case state == machine.target {
        True -> Ok(presses)
        False -> {
          case set.contains(visited, state) {
            True -> bfs(rest, visited, machine)
            False -> {
              let new_states =
                list.map(machine.buttons, fn(button) {
                  let new_state =
                    list.index_map(state, fn(light, i) {
                      case list.contains(button, i) {
                        True -> !light
                        False -> light
                      }
                    })
                  #(new_state, presses + 1)
                })
              bfs(
                list.append(rest, new_states),
                set.insert(visited, state),
                machine,
              )
            }
          }
        }
      }
    }
  }
}

// I identified this being a system of linear equations quite easily, and was about to do the same
// as I did in 2023 day 24 but obviously... no Z3 bindings for Gleam, and implementing Gaussian elimination
// is not straightforward.
// I was looking for maybe an API to do this, or even wondering if I should do a small C/Python local API for this,
// but found the savior for this year AoC -> https://www.reddit.com/r/adventofcode/comments/1pk87hl/2025_day_10_part_2_bifurcate_your_way_to_victory
// This is super well explained, altough the implementation is quite cumbersome, and to be honesty, I relied too much on AI,
// which is probably the reason the code is almost unreadable for part 2.
// Also, thank god that Zed just dropped colorized brackets just in time for this because if not it'd have been HELL.
// So yes, to be completely honest, only about 5% of the code in part 2 is mine.
fn part_2(machines: List(Machine)) -> Int {
  machines
  |> list.map(fn(machine) {
    let #(result, _) = dp_joltage(machine.joltage, machine.buttons, dict.new())
    result
  })
  |> list.fold(0, int.add)
}

// Recursive DP with memoization basically.
// Read the Reddit post linked in the part_2 function to understand this.
// To be honest, the syntax in here is SO BAD, but I was getting tired and frustrated with this
// so I've been patching this with the help of Claude until I got it right.
fn dp_joltage(
  target: List(Int),
  buttons: List(List(Int)),
  cache: dict.Dict(List(Int), Int),
) -> #(Int, dict.Dict(List(Int), Int)) {
  // Base case: all zeros
  case list.all(target, fn(x) { x == 0 }) {
    True -> #(0, cache)
    False -> {
      // Check cache
      case dict.get(cache, target) {
        Ok(result) -> #(result, cache)
        Error(_) -> {
          // Check if any target is negative (which is impossible)
          case list.any(target, fn(x) { x < 0 }) {
            True -> #(1_000_000, cache)
            False -> {
              // Find the target parity
              let target_parity = list.map(target, fn(x) { x % 2 == 1 })

              // Generate all 2^n combinations using fold
              let all_combos =
                list.fold(buttons, [[]], fn(acc, button) {
                  list.flat_map(acc, fn(combo) { [combo, [button, ..combo]] })
                })

              // Filter to valid parity solutions
              let parity_patterns =
                list.filter(all_combos, fn(combo) {
                  let result_parity =
                    list.index_map(target_parity, fn(_, i) {
                      list.count(combo, fn(button) { list.contains(button, i) })
                      % 2
                      == 1
                    })
                  result_parity == target_parity
                })

              // For each parity pattern, calculate the cost
              let #(min_cost, updated_cache) =
                list.fold(
                  parity_patterns,
                  from: #(1_000_000, cache),
                  with: fn(acc, pattern) {
                    let #(best, current_cache) = acc

                    // Apply this pattern once
                    let after_pattern =
                      list.index_map(target, fn(val, i) {
                        let increment =
                          list.count(pattern, fn(button) {
                            list.contains(button, i)
                          })
                        val - increment
                      })

                    // After pressing these buttons once, everything should be even
                    case
                      list.all(after_pattern, fn(x) { x >= 0 && x % 2 == 0 })
                    {
                      True -> {
                        // Divide by 2 and recurse
                        let halved = list.map(after_pattern, fn(x) { x / 2 })
                        let #(sub_cost, new_cache) =
                          dp_joltage(halved, buttons, current_cache)
                        let pattern_cost = list.length(pattern)
                        let total = 2 * sub_cost + pattern_cost
                        #(int.min(best, total), new_cache)
                      }
                      False -> acc
                    }
                  },
                )

              #(min_cost, dict.insert(updated_cache, target, min_cost))
            }
          }
        }
      }
    }
  }
}
