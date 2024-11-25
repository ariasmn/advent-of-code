#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "../helper.h"

#define LAYOUT_MAX_X 110
#define LAYOUT_MAX_Y 110

typedef struct Light
{
    int x;
    int y;
    char direction;
    bool is_finished;
} Light;

int part_1(char **file_content)
{
    // 2D array to represent the layout.
    char layout[LAYOUT_MAX_Y][LAYOUT_MAX_X + 1]; // Null terminator

    for (int i = 0; file_content[i] != NULL; i++)
    {
        for (int j = 0; file_content[i][j] != '\0'; j++)
        {
            layout[i][j] = file_content[i][j];
        }

        layout[i][LAYOUT_MAX_X] = '\0';
    }

    // Define the lights array and add the initial one.
    int lights_arr_size = 1;
    Light *lights = malloc(1 * sizeof(Light));
    Light first_light = {.x = 0, .y = 0, .direction = 'r', .is_finished = false};
    lights[0] = first_light;

    // Visited is a 3D array because we need to hold the directions.
    // We need to do this because we might enter an infinite loop.
    bool visited[LAYOUT_MAX_Y][LAYOUT_MAX_X][4] = {false}; // 4 directions: 0 = 'u', 1 = 'd', 2 = 'l', 3 = 'r'

    // Iterate the array of lights. We might create new lights,
    // but we finish the one that we are currently iterating before moving to
    // another light.
    for (int i = 0; i < lights_arr_size; i++)
    {
        while (!lights[i].is_finished)
        {
            int dir_index;
            switch (lights[i].direction)
            {
            case 'u':
                dir_index = 0;
                break;
            case 'd':
                dir_index = 1;
                break;
            case 'l':
                dir_index = 2;
                break;
            case 'r':
                dir_index = 3;
                break;
            }

            // Check if this position with this direction was already visited.
            if (visited[lights[i].y][lights[i].x][dir_index])
            {
                lights[i].is_finished = true;
                break;
            }

            // Mark this position and direction as visited.
            visited[lights[i].y][lights[i].x][dir_index] = true;

            char current_cell = layout[lights[i].y][lights[i].x];

            // Change direction and/or create new light based on current cell.
            if (current_cell == '/')
            {
                if (lights[i].direction == 'r')
                {
                    lights[i].direction = 'u';
                }
                else if (lights[i].direction == 'l')
                {
                    lights[i].direction = 'd';
                }

                else if (lights[i].direction == 'u')
                {
                    lights[i].direction = 'r';
                }

                else if (lights[i].direction == 'd')
                {
                    lights[i].direction = 'l';
                }
            }
            else if (current_cell == '\\')
            {
                if (lights[i].direction == 'r')
                {
                    lights[i].direction = 'd';
                }
                else if (lights[i].direction == 'l')
                {
                    lights[i].direction = 'u';
                }

                else if (lights[i].direction == 'u')
                {
                    lights[i].direction = 'l';
                }

                else if (lights[i].direction == 'd')
                {
                    lights[i].direction = 'r';
                }
            }
            else if (current_cell == '|' && (lights[i].direction == 'r' || lights[i].direction == 'l'))
            {
                // Handle vertical splits.
                lights[i].direction = 'u';
                Light new_light = {.x = lights[i].x, .y = lights[i].y, .direction = 'd', .is_finished = false};
                lights = realloc(lights, (lights_arr_size + 1) * sizeof(Light));
                lights[lights_arr_size] = new_light;
                lights_arr_size++;
            }
            else if (current_cell == '-' && (lights[i].direction == 'u' || lights[i].direction == 'd'))
            {
                // Handle horizontal splits.
                lights[i].direction = 'l';
                Light new_light = {.x = lights[i].x, .y = lights[i].y, .direction = 'r', .is_finished = false};
                lights = realloc(lights, (lights_arr_size + 1) * sizeof(Light));
                lights[lights_arr_size] = new_light;
                lights_arr_size++;
            }

            // Calculate next position.
            switch (lights[i].direction)
            {
            case 'u':
                lights[i].y -= 1;
                break;
            case 'd':
                lights[i].y += 1;
                break;
            case 'l':
                lights[i].x -= 1;
                break;
            case 'r':
                lights[i].x += 1;
                break;
            }

            // Stop if out of bounds.
            if (lights[i].y >= LAYOUT_MAX_Y || lights[i].y < 0 || lights[i].x >= LAYOUT_MAX_X || lights[i].x < 0)
            {
                lights[i].is_finished = true;
            }
        }
    }

    // Count how many tiles we visited.
    int count = 0;
    for (int y = 0; y < LAYOUT_MAX_Y; y++)
    {
        for (int x = 0; x < LAYOUT_MAX_X; x++)
        {
            // Check if any direction has been visited at this tile.
            if (visited[y][x][0] || visited[y][x][1] || visited[y][x][2] || visited[y][x][3])
            {
                count++;
            }
        }
    }

    return count;
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
