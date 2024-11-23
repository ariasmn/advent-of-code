#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#include "../helper.h"

#define DISH_MAX_X 100
#define DISH_MAX_Y 100

// If it's a '0', we try to move up.
// If it's a '#', we do nothing, and nothing can't be placed there.
// If it's a '.', we do nothing, but a '0' can be placed there.
// We start at 1, since nothing can be moved upper in 0.
void tilt_north(char dish[DISH_MAX_Y][DISH_MAX_X + 1])
{
    for (int i = 1; i < DISH_MAX_Y; i++)
    {
        for (int j = 0; j < DISH_MAX_X; j++)
        {
            // We only need to do something if it's a rock.
            if (dish[i][j] != 'O')
            {
                continue;
            }

            // We analyze from current's above line to most top, to see where the rock will stay.
            for (int k = i - 1; k >= 0; k--)
            {
                // We move the rock if there is a space.
                // If there's a block or another rock, we can't go up any further, so we stop checking.
                if (dish[k][j] != '.')
                {
                    break;
                }
                dish[k][j] = 'O';
                dish[k + 1][j] = '.';
            }
        }
    }
}

// Function that rotates the dish (matrix) 90° clockwise.
// What it essentially does is just transposing the matrix and reversing each row.
void rotate_dish(char dish[DISH_MAX_Y][DISH_MAX_X + 1])
{
    // Step 1: Transpose the matrix
    for (int i = 0; i < DISH_MAX_Y; i++)
    {
        for (int j = i + 1; j < DISH_MAX_Y; j++)
        {
            char temp = dish[i][j];
            dish[i][j] = dish[j][i];
            dish[j][i] = temp;
        }
    }

    // Step 2: Reverse each row
    for (int i = 0; i < DISH_MAX_Y; i++)
    {
        for (int j = 0, k = DISH_MAX_Y - 1; j < k; j++, k--)
        {
            char temp = dish[i][j];
            dish[i][j] = dish[i][k];
            dish[i][k] = temp;
        }
    }
}

int part_1(char **file_content)
{
    // Create a 2D array that we will use to move the rocks.
    char dish[DISH_MAX_X][DISH_MAX_Y + 1]; // Null terminator

    for (int i = 0; file_content[i] != NULL; i++)
    {
        for (int j = 0; file_content[i][j] != '\0'; j++)
        {
            dish[i][j] = file_content[i][j];
        }

        dish[i][DISH_MAX_Y] = '\0';
    }

    // Iterate the dish and do the logic
    tilt_north(dish);

    int total = 0;
    // Now, the dish is formatted and we just need to calculate.
    for (int i = 0; i < DISH_MAX_Y; i++)
    {
        int load = DISH_MAX_Y - i;

        for (int j = 0; j < DISH_MAX_X; j++)
        {
            if (dish[i][j] == 'O')
            {
                total += load;
            }
        }
    }

    return total;
}

int part_2(char **file_content)
{
    // Just copy and paste the parsing that I used for part_1.
    char dish[DISH_MAX_X][DISH_MAX_Y + 1]; // Null terminator
    for (int i = 0; i < DISH_MAX_X && file_content[i] != NULL; i++)
    {
        for (int j = 0; j < DISH_MAX_Y && file_content[i][j] != '\0'; j++)
        {
            dish[i][j] = file_content[i][j];
        }
        dish[i][DISH_MAX_Y] = '\0'; // Null-terminate each row safely
    }

    // Since we can't do the cycle a billion times, we need to store previous dish states.
    // This is called "Cycle detection", and we are using a simplified "Tortoise and Hare" algorithm.
    char (*seen_states)[DISH_MAX_X][DISH_MAX_Y + 1] = malloc(100000 * sizeof(char[DISH_MAX_X][DISH_MAX_Y + 1]));
    int states_count = 0;
    int cycle_start = -1;
    int cycle_length = -1;
    bool cycle_found = false;

    // Run until we find a cycle or hit a reasonable maximum
    // We need to tilt north, west, south and east.
    // Meaning, we can rotate the dish and tilt north and it'll be basically the same.
    // This is for each cycle, so a single cycle will do 4 tilts and 4 rotations.
    for (int i = 0; i < 100000 && !cycle_found; i++)
    {
        // Perform one complete cycle
        for (int j = 0; j < 4; j++)
        {
            tilt_north(dish);
            rotate_dish(dish);
        }

        // Check if we've seen this state before
        // We could probably use some sort of hash for the 2D array instead of this but whatever, it works!
        for (int k = 0; k < states_count; k++)
        {
            bool match = true;
            for (int x = 0; x < DISH_MAX_X && match; x++)
            {
                for (int y = 0; y < DISH_MAX_Y && match; y++)
                {
                    if (dish[x][y] != seen_states[k][x][y])
                    {
                        match = false;
                        break;
                    }
                }
            }
            // If we do, we now know where the cycle starts and its length.
            if (match)
            {
                cycle_start = k;
                cycle_length = i - k;
                cycle_found = true;
                break;
            }
        }

        // If we get there and there's no cycle found, we need to store the
        // state so we can compare it in future cycles.
        if (!cycle_found)
        {
            // Store current state.
            for (int x = 0; x < DISH_MAX_X; x++)
            {
                for (int y = 0; y < DISH_MAX_Y; y++)
                {
                    seen_states[states_count][x][y] = dish[x][y];
                }
            }
            states_count++;
        }
    }

    if (cycle_found)
    {
        // Calculate remaining cycles needed.
        long long target = 1000000000;

        // -1 because of 0 indexing. Might seem stupid but took long to debug because of this...
        long long remaining = (target - cycle_start - 1) % cycle_length;
        
        // Run the remaining cycles
        for (long long i = 0; i < remaining; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                tilt_north(dish);
                rotate_dish(dish);
            }
        }
    }

    free(seen_states);

    int total = 0;
    for (int i = 0; i < DISH_MAX_Y; i++)
    {
        int load = DISH_MAX_Y - i;
        for (int j = 0; j < DISH_MAX_X; j++)
        {
            if (dish[i][j] == 'O')
            {
                total += load;
            }
        }
    }

    return total;
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
    printf("Part 2: %d\n", part_2(file_content));
}