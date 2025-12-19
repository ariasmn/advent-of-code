import gleam/format
import gleam/int
import gleam/list
import gleam/set
import gleam/string
import simplifile

// We are saving the area of both presents and trees, you'll understand why.
pub type Present {
  Present(coordinates: set.Set(#(Int, Int)), area: Int)
}

pub type Tree {
  Tree(presents: List(Int), area: Int)
}

pub fn main() {
  let assert Ok(content) = simplifile.read(from: "./src/day_12/input.txt")
  let sections =
    content
    |> string.split("\n\n")

  // Separate presents sections from the regions section (last section)
  let #(presents_sections, regions_section) = case list.reverse(sections) {
    [last, ..rest] -> #(list.reverse(rest), last)
    [] -> #([], "")
  }

  let presents =
    list.map(presents_sections, fn(section) {
      let lines = string.split(section, "\n")
      let assert [_first, ..grid_lines] = lines

      let coordinates =
        list.index_fold(grid_lines, set.new(), fn(acc, line, y) {
          list.index_fold(string.to_graphemes(line), acc, fn(acc2, char, x) {
            case char {
              "#" -> set.insert(acc2, #(x, y))
              _ -> acc2
            }
          })
        })

      Present(coordinates: coordinates, area: set.size(coordinates))
    })

  let trees =
    regions_section
    |> string.split("\n")
    |> list.filter(fn(line) { line != "" })
    |> list.map(fn(line) {
      let assert [size_part, presents_part] = string.split(line, ": ")
      let assert [width_str, height_str] = string.split(size_part, "x")
      let assert Ok(width) = int.parse(width_str)
      let assert Ok(height) = int.parse(height_str)

      let present_counts =
        string.split(presents_part, " ")
        |> list.map(fn(p) {
          let assert Ok(num) = int.parse(p)
          num
        })

      Tree(presents: present_counts, area: width * height)
    })

  format.printf("Part 1: ~b\n", part_1(presents, trees))
}

// This is a well known NP-Hard problem, so I knew that there should be a trick
// regarding the input, but to be honest, I couldn't really find it.
// Main problem is that I focused too much on the test input (as always),
// and for this, the test input is way harder than the main input, because the "trick"
// is only in your main input.
// I took at look at a website link I found in Reddit or GitHub, can't remember
// https://notes.hamatti.org/technology/advent-of-code/2025/day-12 which explain quite good how this works.
// Basically, by the calculated area, you just need to check if they fill. No need to check rotated or anything.
fn part_1(presents: List(Present), trees: List(Tree)) -> Int {
  list.fold(trees, 0, fn(valid_trees, tree) {
    let area_needed =
      list.fold(list.zip(tree.presents, presents), 0, fn(acc, pair) {
        let #(count, present) = pair
        acc + count * present.area
      })

    case tree.area >= area_needed {
      True -> valid_trees + 1
      False -> valid_trees
    }
  })
}
