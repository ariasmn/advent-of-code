import gleam/format
import gleam/int
import gleam/io
import gleam/list
import gleam/set
import gleam/string
import simplifile

type Machine {
  Machine(target: List(Bool), buttons: List(List(Int)))
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

      Machine(target, buttons)
    })

  format.printf("Part 1: ~b\n", part_1(machines))
  format.printf("Part 2: ~b\n", part_2(machines))
}

// I solved this by using good old BFS (this feels like home in a AoC context :D).
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

fn part_2(_machines: List(Machine)) -> Int {
  0
}
