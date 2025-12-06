import gleam/format
import gleam/int
import gleam/list
import gleam/string
import simplifile

pub fn main() {
  let assert Ok(content) = simplifile.read(from: "./src/day_06/input.txt")

  format.printf("Part 1: ~b\n", part_1(content))
  format.printf("Part 2: ~b\n", part_2(content))
}

fn part_1(content: String) -> Int {
  let lines = string.split(content, "\n")
  // This is pretty easy if you just transpose the input and parse each operation
  // as columns instead of rows.
  let transposed_lines =
    lines
    |> list.map(fn(line) {
      string.split(line, " ")
      |> list.filter(fn(cell) { cell != "" })
    })
    // Already builtin function to do it. Godsend!
    |> list.transpose()

  let result =
    transposed_lines
    |> list.fold(from: 0, with: fn(acc, operation_line) {
      // The last character is the operation that we need to do, and the others is just the operands.
      // Get the operator to either multiply or sum.
      let assert Ok(operator) = list.last(operation_line)
      // Get all the operands.
      let operands = list.take(operation_line, list.length(operation_line) - 1)
      let line_result = case operator {
        "*" -> {
          list.fold(operands, from: 1, with: fn(res, num) {
            let assert Ok(parsed_number) = int.parse(num)
            res * parsed_number
          })
        }
        "+" -> {
          list.fold(operands, from: 0, with: fn(res, num) {
            let assert Ok(parsed_number) = int.parse(num)
            res + parsed_number
          })
        }
        _ -> 0
      }
      // Sum the operations that we're doing.
      acc + line_result
    })
  result
}

fn part_2(content: String) -> Int {
  let lines = string.split(content, "\n") |> list.filter(fn(l) { l != "" })
  // Split into character grid.
  // This means, create a list in the same way we had the numbers (by rows),
  // but having each individual character.
  let grid = list.map(lines, string.to_graphemes)
  // Transpose to get columns.
  // When you transpose, we have the same as we had in the grid, but
  // like read in columns.
  let columns = list.transpose(grid)

  // Remove a group with no info (this means, a column that is all whitespaces)
  // and group by operations.
  // This is now a List(List(List(String))), which basically translates to:
  //
  // 1. List: The full collection of all problems.
  // 2. Group: The mathematical expresion.
  // 3. Column: One vertical column of characters within that problem.
  // 4. Grapheme (String): The single character at a specific row/column.
  let final_groups =
    list.fold(columns, from: #([], []), with: fn(acc, col) {
      let #(all_groups, current_group) = acc
      // Check if this column is all spaces
      let is_space_col = list.all(col, fn(c) { c == " " })
      case is_space_col {
        True -> #(list.append(all_groups, [current_group]), [])
        False -> #(all_groups, list.append(current_group, [col]))
      }
    })
    |> fn(result) {
      let #(all_groups, last_group) = result
      list.append(all_groups, [last_group])
    }
    |> list.filter(fn(g) { g != [] })

  // Process each group
  list.fold(final_groups, from: 0, with: fn(acc, group) {
    // Always the operator is in the last grapheme of the first column of the group.
    let assert Ok(first_col) = list.first(group)
    let assert Ok(operator) = list.last(first_col)
    // Each column becomes a number
    let numbers =
      group
      |> list.index_map(fn(col, idx) {
        // Only first column has operator at the end, others use all chars
        let digits = case idx {
          0 -> list.take(col, list.length(col) - 1)
          _ -> col
        }
        digits
        |> list.filter(fn(c) { c != " " })
        |> string.join("")
      })
      |> list.filter(fn(s) { s != "" })
      |> list.map(fn(num_str) {
        let assert Ok(n) = int.parse(num_str)
        n
      })
    let result = case operator {
      "*" -> list.fold(numbers, from: 1, with: int.multiply)
      "+" -> list.fold(numbers, from: 0, with: int.add)
      _ -> 0
    }
    acc + result
  })
}
