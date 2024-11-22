#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

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

int main()
{
    FILE *fptr = fopen("input.txt", "r");
    char **file_content = read_file(fptr);

    if (file_content == NULL)
    {
        return 1;
    }

    printf("Part 1: %d\n", part_1(file_content));
    // printf("Part 2: %d\n", part_2(file_content));
}