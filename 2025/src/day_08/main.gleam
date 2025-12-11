import gleam/dict
import gleam/float
import gleam/format
import gleam/int
import gleam/list
import gleam/string
import simplifile

pub type Box {
  Box(x: Int, y: Int, z: Int)
}

// This is the number of connections. Changes between test input and your input,
// so remember to change this before running.
const num_connections = 1000

pub fn main() {
  let assert Ok(content) = simplifile.read(from: "./src/day_08/input.txt")
  // Parse into a custom box type. We could use a tuple with three values, but want to use a custom
  // type before ending AoC.
  let boxes =
    content
    |> string.split("\n")
    |> list.filter(fn(line) { line != "" })
    |> list.map(fn(unparsed_box) {
      let assert [x_str, y_str, z_str] = string.split(unparsed_box, ",")
      let assert Ok(x) = int.parse(x_str)
      let assert Ok(y) = int.parse(y_str)
      let assert Ok(z) = int.parse(z_str)
      Box(x, y, z)
    })

  format.printf("Part 1: ~b\n", part_1(boxes))
  format.printf("Part 2: ~b\n", part_2(boxes))
}

// This came in clutch since I actually finished studying about disjoint set and the
// Kruskal algorithm like 2 months ago (had to do Kruskal in paper and pencil)
// so this seemed pretty familiar actually!
fn part_1(boxes: List(Box)) -> Int {
  // Create a list of tuples with 2 boxes and euclidean distance between them
  let pairs_with_distances =
    list.combination_pairs(boxes)
    |> list.map(fn(pair) {
      let #(b1, b2) = pair
      #(b1, b2, euclidean_distance(b1, b2))
    })
    // Sort by distance (closest first)
    // and take the number of connections we're interested in.
    |> list.sort(fn(a, b) {
      let #(_, _, dist_a) = a
      let #(_, _, dist_b) = b
      float.compare(dist_a, dist_b)
    })
    |> list.take(num_connections)

  // We create the INITIAL Union-Find data structure, which is
  // essentially a dict that indicates that it's box is in its own circuit.
  // https://en.wikipedia.org/wiki/Disjoint-set_data_structure
  let initial_uf =
    list.fold(boxes, from: dict.new(), with: fn(uf, box) {
      dict.insert(uf, box, box)
    })

  // Process connections using union-find
  // Basically, we iterate over the pairs we took and check if they share the same root.
  // If they do, they are in the same circuit.
  // After this, we get a dict in which the key indicates a box, and the value is the parent box.
  let final_uf =
    list.fold(pairs_with_distances, from: initial_uf, with: fn(uf, conn) {
      let #(b1, b2, _) = conn

      // Find roots
      let root1 = find_root(uf, b1)
      let root2 = find_root(uf, b2)

      // This is the key here:
      // If the root of each box is the same, it means that it's already inside the same circuit
      // if not, we need to connect them, because from the input we do know that these are connected, since
      // we are only taking the pairs with the shortest euclidean distance.
      case root1 == root2 {
        // Already connected
        True -> uf
        False -> dict.insert(uf, root1, root2)
      }
    })

  // Count how many boxes belong to each circuit.
  // All boxes in a circuit share the same root, so we group by root.
  let circuit_sizes =
    list.fold(boxes, from: dict.new(), with: fn(sizes, box) {
      let root = find_root(final_uf, box)
      let current_size = case dict.get(sizes, root) {
        Ok(size) -> size
        _ -> 0
      }
      dict.insert(sizes, root, current_size + 1)
    })
    |> dict.values()

  // Get 3 largest and multiply
  let top_3 =
    circuit_sizes
    |> list.sort(int.compare)
    |> list.reverse()
    |> list.take(3)

  case top_3 {
    [a, b, c] -> a * b * c
    _ -> 0
  }
}

// We process connections in order of distance. For each connection,
// we check if the two boxes are already in the same circuit.
// If not, we connect them and track this as the "last" connection made.
fn part_2(boxes: List(Box)) -> Int {
  // Create all pairs with distances and sort by distance
  let pairs_with_distances =
    list.combination_pairs(boxes)
    |> list.map(fn(pair) {
      let #(b1, b2) = pair
      #(b1, b2, euclidean_distance(b1, b2))
    })
    |> list.sort(fn(a, b) {
      let #(_, _, dist_a) = a
      let #(_, _, dist_b) = b
      float.compare(dist_a, dist_b)
    })

  // Initialize union-find with each box in its own circuit
  let initial_uf =
    list.fold(boxes, from: dict.new(), with: fn(uf, box) {
      dict.insert(uf, box, box)
    })

  // Get the first pair as the initial connection
  let assert Ok(first_pair) = list.first(pairs_with_distances)
  let #(first_b1, first_b2, _) = first_pair
  let initial_state = #(initial_uf, first_b1, first_b2)

  let final_state =
    list.fold(pairs_with_distances, from: initial_state, with: fn(state, conn) {
      // Unpack current state
      let #(uf, _prev_b1, _prev_b2) = state
      let #(b1, b2, _dist) = conn

      // Find roots of both boxes
      let root1 = find_root(uf, b1)
      let root2 = find_root(uf, b2)

      // Check if they're already in the same circuit
      case root1 == root2 {
        // Skip this connection
        True -> state
        // Connect the two circuits by making root1 point to root2
        False -> {
          let new_uf = dict.insert(uf, root1, root2)

          // Return updated state with this connection as the "last" one
          let new_state = #(new_uf, b1, b2)
          new_state
        }
      }
    })

  // Extract the last two boxes that were connected
  let #(_final_uf, last_b1, last_b2) = final_state

  // Return the product of X coordinates
  last_b1.x * last_b2.x
}

// Recursively find the root of a box in the union-find structure
fn find_root(uf: dict.Dict(Box, Box), box: Box) -> Box {
  let assert Ok(parent) = dict.get(uf, box)
  case parent == box {
    True -> box
    False -> find_root(uf, parent)
  }
}

// Function that calculates the euclidean distance between two boxes.
// Formula -> https://en.wikipedia.org/wiki/Euclidean_distance#Higher_dimensions
// It exists in a community package -> https://hexdocs.pm/gleam_community_maths/gleam_community/maths.html#euclidean_distance
// but it's simple enough to do it myself and it gives me a warning for some reason...
fn euclidean_distance(b1: Box, b2: Box) -> Float {
  let dx = int.to_float(b2.x - b1.x)
  let dy = int.to_float(b2.y - b1.y)
  let dz = int.to_float(b2.z - b1.z)

  let assert Ok(dist) = float.square_root(dx *. dx +. dy *. dy +. dz *. dz)
  dist
}
