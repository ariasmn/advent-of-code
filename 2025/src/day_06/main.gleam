import gleam/format
import gleam/int
import gleam/list
import gleam/string
import simplifile

pub fn main() {
  let assert Ok(content) = simplifile.read(from: "./src/day_06/input.txt")
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
  // |> echo

  format.printf("Part 1: ~b\n", part_1(transposed_lines))
}

fn part_1(lines: List(List(String))) -> Int {
  let result =
    lines
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
