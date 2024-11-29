// DISCLAIMER: Starting day 21, I've been just looking at solutions from another languages I know (Golang, PHP...)
// and basically doing it in my own way in C.
// I looked at different solutions and took ideas from all of them to make my own version, so I can't link to the solution directly,
// but all of the solutions can be found in the subreddit: https://www.reddit.com/r/adventofcode/.
// When I started AoC 2023, it was because I wanted to learn C and
// honestly, I wasn't in the mood to solve certain puzzles (I got a bit tired of it in AoC 2022).
// So, since I struggled A LOT in day 20, I've decided to just do it this way.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "../helper.h"

#define GRID_SIZE 131
#define STEPS 64

// Directions: north, south, east, west
int row_directions[] = {-1, 1, 0, 0};
int col_directions[] = {0, 0, 1, -1};

// Checks if a position is within grid bounds.
bool is_valid(int row, int col)
{
    return row >= 0 && row < GRID_SIZE && col >= 0 && col < GRID_SIZE;
}

//Calculates the number of garden plots that can be reached in exactly 'steps' moves
// from a starting position in a given grid.
int count_reachable(char grid[GRID_SIZE][GRID_SIZE], int start_row, int start_col, int steps)
{
    bool current[GRID_SIZE][GRID_SIZE] = {false};
    bool next[GRID_SIZE][GRID_SIZE] = {false};

    // Start with initial position.
    current[start_row][start_col] = true;

    // For each step.
    for (int i = 0; i < steps; i++)
    {
        memset(next, 0, sizeof(next));

        // For each current position.
        for (int r = 0; r < GRID_SIZE; r++)
        {
            for (int c = 0; c < GRID_SIZE; c++)
            {
                if (!current[r][c])
                    continue;

                // Try all four directions.
                int dirs[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
                for (int d = 0; d < 4; d++)
                {
                    int new_row = r + dirs[d][0];
                    int new_col = c + dirs[d][1];

                    // Check boundaries and walls.
                    if (new_row >= 0 && new_col >= 0 && new_row < GRID_SIZE && new_col < GRID_SIZE &&
                        grid[new_row][new_col] != '#')
                    {
                        next[new_row][new_col] = true;
                    }
                }
            }
        }
        // Swap buffers.
        memcpy(current, next, sizeof(current));
    }

    // Count final positions.
    int count = 0;
    for (int r = 0; r < GRID_SIZE; r++)
    {
        for (int c = 0; c < GRID_SIZE; c++)
        {
            if (current[r][c])
                count++;
        }
    }

    return count;
}

int part_1(char **file_content)
{
    char grid[GRID_SIZE][GRID_SIZE];
    int start_row = 0, start_column = 0;

    // Copy grid and find start.
    for (int r = 0; r < GRID_SIZE; r++)
    {
        for (int c = 0; c < GRID_SIZE; c++)
        {
            grid[r][c] = file_content[r][c];
            if (grid[r][c] == 'S')
            {
                start_row = r;
                start_column = c;
                grid[r][c] = '.'; // Replace S with .
            }
        }
    }

    return count_reachable(grid, start_row, start_column, STEPS);
}


// Key insights for solving part 2. Obviously, had to look up Reddit for hints.
// Not only hints, but this code is also heavily inspired on a Golang code I saw on a comment. Can't link
// the code because I lost it through a sea of tabs...
// 1. The input grid (131x131) repeats infinitely in all directions
// 2. The target steps (26501365) can be written as: 202300 * 131 + 65
//    This means after filling each grid completely, we form a diamond pattern
// 
// The solution splits the problem into:
// a) Complete grids: These are fully filled grids that we encounter
//    - Odd-step grids: Grids reached in odd number of steps
//    - Even-step grids: Grids reached in even number of steps
//    The number of odd and even grids forms a pattern based on the number of complete grid repeats
// 
// b) Edge cases: The grids at the boundary of our diamond
//    - Cardinal directions: Points reached from the center grid's edges
//    - Small corners: Points reached near the corners (using grid_size/2 - 1 steps)
//    - Big corners: Points reached further in corners (using grid_size*3/2 - 1 steps)
long long int part_2(char **file_content) {
    char grid[GRID_SIZE][GRID_SIZE];
    int start_r = 0, start_c = 0;
    
    for (int r = 0; r < GRID_SIZE; r++) {
        for (int c = 0; c < GRID_SIZE; c++) {
            grid[r][c] = file_content[r][c];
            if (grid[r][c] == 'S') {
                start_r = r;
                start_c = c;
            }
        }
    }
    
    long long grids = 26501365LL / GRID_SIZE - 1;
    
    // Calculate odd and even grids.
    long long odd_side = (grids / 2) * 2 + 1;
    long long even_side = ((grids + 1) / 2) * 2;
    long long odd_grids = odd_side * odd_side;
    long long even_grids = even_side * even_side;
    
    // Main contributions (these were correct).
    long long result = 
        odd_grids * count_reachable(grid, start_r, start_c, GRID_SIZE * 2 + 1) +
        even_grids * count_reachable(grid, start_r, start_c, GRID_SIZE * 2);
    
    // Cardinal directions.
    long long cardinal_sum = 
        count_reachable(grid, GRID_SIZE-1, start_c, GRID_SIZE - 1) +
        count_reachable(grid, start_r, 0, GRID_SIZE - 1) +
        count_reachable(grid, 0, start_c, GRID_SIZE - 1) +
        count_reachable(grid, start_r, GRID_SIZE-1, GRID_SIZE - 1);
    result += cardinal_sum;
    
    // Small corners (with half grid steps).
    long long small_corners = 
        count_reachable(grid, 0, GRID_SIZE-1, GRID_SIZE/2 - 1) +
        count_reachable(grid, GRID_SIZE-1, GRID_SIZE-1, GRID_SIZE/2 - 1) +
        count_reachable(grid, 0, 0, GRID_SIZE/2 - 1) +
        count_reachable(grid, GRID_SIZE-1, 0, GRID_SIZE/2 - 1);
    result += (grids + 1) * small_corners;
    
    // Big corners (with 3/2 grid steps).
    long long big_corners = 
        count_reachable(grid, 0, GRID_SIZE-1, (GRID_SIZE * 3)/2 - 1) +
        count_reachable(grid, GRID_SIZE-1, GRID_SIZE-1, (GRID_SIZE * 3)/2 - 1) +
        count_reachable(grid, 0, 0, (GRID_SIZE * 3)/2 - 1) +
        count_reachable(grid, GRID_SIZE-1, 0, (GRID_SIZE * 3)/2 - 1);
    result += grids * big_corners;
    
    return result;
}

int main()
{
    FILE *fptr = fopen("input.txt", "r");
    char **file_content = read_file(fptr);

    if (file_content == NULL)
    {
        return 1;
    }

    printf("Part 1: %d\n", part_1(file_content));
    printf("Part 2: %lld\n", part_2(file_content));
}
