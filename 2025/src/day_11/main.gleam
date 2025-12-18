import gleam/dict
import gleam/format
import gleam/list
import gleam/set
import gleam/string
import simplifile

pub fn main() {
  let assert Ok(content) = simplifile.read(from: "./src/day_11/input.txt")
  let devices =
    content
    |> string.split("\n")
    |> list.filter(fn(line) { line != "" })
    |> list.fold(from: dict.new(), with: fn(acc, line) {
      let assert [node, connections_str] = string.split(line, ": ")
      let connections =
        string.split(connections_str, " ")
        |> list.filter(fn(neighbor) { neighbor != node })
      dict.insert(acc, node, connections)
    })

  format.printf("Part 1: ~b\n", part_1(devices))
  format.printf("Part 2: ~b\n", part_2(devices))
}

// This part is just doing DFS, so quite easy.
fn part_1(devices: dict.Dict(String, List(String))) -> Int {
  let paths = dfs("you", "out", devices, [])
  list.length(paths)
}

fn dfs(
  current: String,
  target: String,
  devices: dict.Dict(String, List(String)),
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

// I didn't think this would work, but it's that easy...
// Just add memoization and use two bools to check if we have seen dac and fft in the path.
// Maybe for part 2 I should've created a custom type but this works so...
fn part_2(devices: dict.Dict(String, List(String))) -> Int {
  let #(count, _) =
    dfs_cached("svr", "out", devices, set.new(), False, False, dict.new())
  count
}

fn dfs_cached(
  current: String,
  target: String,
  devices: dict.Dict(String, List(String)),
  visited: set.Set(String),
  seen_dac: Bool,
  seen_fft: Bool,
  cache: dict.Dict(#(String, Bool, Bool), Int),
) -> #(Int, dict.Dict(#(String, Bool, Bool), Int)) {
  let current_seen_dac = seen_dac || current == "dac"
  let current_seen_fft = seen_fft || current == "fft"

  let cache_key = #(current, current_seen_dac, current_seen_fft)

  case current == target {
    True -> {
      // Only count if we've seen both dac and fft
      case current_seen_dac && current_seen_fft {
        True -> #(1, cache)
        False -> #(0, cache)
      }
    }
    False -> {
      case dict.get(cache, cache_key) {
        Ok(cached) -> #(cached, cache)
        Error(_) -> {
          case set.contains(visited, current) {
            True -> #(0, cache)
            False -> {
              case dict.get(devices, current) {
                Ok(neighbors) -> {
                  let new_visited = set.insert(visited, current)
                  let #(total, final_cache) =
                    list.fold(neighbors, #(0, cache), fn(acc, neighbor) {
                      let #(count_so_far, current_cache) = acc
                      let #(neighbor_count, updated_cache) =
                        dfs_cached(
                          neighbor,
                          target,
                          devices,
                          new_visited,
                          current_seen_dac,
                          current_seen_fft,
                          current_cache,
                        )
                      #(count_so_far + neighbor_count, updated_cache)
                    })
                  #(total, dict.insert(final_cache, cache_key, total))
                }
                Error(_) -> #(0, cache)
              }
            }
          }
        }
      }
    }
  }
}
