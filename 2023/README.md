# Advent of Code 2023 in C

## Setup & Running

1. Add your input data:
   - Place an `input.txt` file in the folder of the day you want to solve.

2. Compile and run solutions:
   ```bash
   gcc -o my_program main.c ../helper.c && ./my_program && rm my_program
   ```

## Special Requirements

Day 24 requires the additional library Z3 theorem prover/solver:

1. Install Z3 (Fedora example):
   ```bash
   sudo dnf install z3 z3-devel
   ```

2. Compile and run with Z3:
   ```bash
   gcc -o my_program -lz3 main.c ../helper.c && ./my_program && rm my_program
   ```