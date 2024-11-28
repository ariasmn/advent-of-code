#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "../helper.h"

#define GRID_ROWS 131
#define GRID_COLUMNS 131 // +1 because of the null terminator.
#define STEPS 64

// Directions: north, south, east, west
int row_directions[] = {-1, 1, 0, 0};
int col_directions[] = {0, 0, 1, -1};

// Checks if a position is within grid bounds.
bool is_valid(int row, int col)
{
    return row >= 0 && row < GRID_ROWS && col >= 0 && col < GRID_COLUMNS;
}

int part_1(char **file_content)
{
    char grid[GRID_ROWS][GRID_COLUMNS];
    bool current_step[GRID_ROWS][GRID_COLUMNS] = {false};

    // Copy grid
    for (int i = 0; file_content[i] != NULL; i++)
    {
        strncpy(grid[i], file_content[i], GRID_COLUMNS);
        grid[i][GRID_COLUMNS] = '\0';
    }

    // Find starting position
    int start_row = 0, start_column = 0;
    for (int r = 0; r < GRID_ROWS; r++)
    {
        for (int c = 0; c < GRID_COLUMNS; c++)
        {
            if (grid[r][c] == 'S')
            {
                start_row = r;
                start_column = c;
                grid[r][c] = '.'; // Replace S with .
                break;
            }
        }
    }

    // Mark start position for first step
    current_step[start_row][start_column] = true;

    // Perform steps
    for (int step = 0; step < STEPS; step++)
    {
        bool next_step[GRID_ROWS][GRID_COLUMNS] = {false};

        for (int r = 0; r < GRID_ROWS; r++)
        {
            for (int c = 0; c < GRID_COLUMNS; c++)
            {
                if (current_step[r][c])
                {
                    // Check all 4 directions
                    for (int i = 0; i < 4; i++)
                    {
                        int new_r = r + row_directions[i];
                        int new_c = c + col_directions[i];

                        if (is_valid(new_r, new_c) &&
                            grid[new_r][new_c] == '.')
                        {
                            next_step[new_r][new_c] = true;
                        }
                    }
                }
            }
        }

        // Copy next step to current step
        memcpy(current_step, next_step, sizeof(current_step));
    }

    // Count reachable positions
    int result = 0;
    for (int r = 0; r < GRID_ROWS; r++)
    {
        for (int c = 0; c < GRID_COLUMNS; c++)
        {
            if (current_step[r][c])
            {
                result++;
            }
        }
    }

    return result;
}

long long int part_2(char **file_content)
{
    return 0;
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
