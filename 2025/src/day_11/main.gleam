import gleam/dict.{type Dict}
import gleam/format
import gleam/list
import gleam/string
import simplifile

pub fn main() {
  let assert Ok(content) = simplifile.read(from: "./src/day_11/input.txt")

  // A dict with each device and a list of the devices that connects to.
  let devices =
    content
    |> string.split("\n")
    |> list.filter(fn(line) { line != "" })
    |> list.fold(from: dict.new(), with: fn(acc, line) {
      let assert [node, connections_str] = string.split(line, ": ")
      let connections = string.split(connections_str, " ")
      dict.insert(acc, node, connections)
    })

  format.printf("Part 1: ~b\n", part_1(devices))
}

// This part is just doing DFS, so quite easy.
fn part_1(devices: Dict(String, List(String))) -> Int {
  let paths = dfs("you", "out", devices, [])
  list.length(paths)
}

fn dfs(
  current: String,
  target: String,
  devices: Dict(String, List(String)),
  visited: List(String),
) -> List(List(String)) {
  case current == target {
    True -> [[target]]
    False -> {
      case list.contains(visited, current) {
        True -> []
        False -> {
          let assert Ok(neighbors) = dict.get(devices, current)
          list.flat_map(neighbors, fn(neighbor) {
            let paths_from_neighbor =
              dfs(neighbor, target, devices, [current, ..visited])
            list.map(paths_from_neighbor, fn(path) { [current, ..path] })
          })
        }
      }
    }
  }
}
