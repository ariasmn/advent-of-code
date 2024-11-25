#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "../helper.h"

#define LAYOUT_MAX_X 110
#define LAYOUT_MAX_Y 110

typedef enum
{
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

typedef struct
{
    int x;
    int y;
    Direction direction;
    bool is_finished;
} Light;

// process_lights holds the whole problem logic.
// This is done in a separate function so we can reuse for part 2.
int process_light(char layout[LAYOUT_MAX_X][LAYOUT_MAX_Y +1], Light initial_light)
{
    // Define the lights array and add the initial one.
    int lights_arr_size = 1;
    Light *lights = malloc(1 * sizeof(Light));
    lights[0] = initial_light;

    // Visited is a 3D array because we need to hold the directions.
    // We need to do this because we might enter an infinite loop.
    bool visited[LAYOUT_MAX_Y][LAYOUT_MAX_X][4] = {false};

    // Iterate the array of lights. We might create new lights,
    // but we finish the one that we are currently iterating before moving to
    // another light.
    for (int i = 0; i < lights_arr_size; i++)
    {
        while (!lights[i].is_finished)
        {
            Direction dir_index = lights[i].direction;

            // Check if this position with this direction was already visited.
            if (visited[lights[i].y][lights[i].x][dir_index])
            {
                lights[i].is_finished = true;
                break;
            }

            visited[lights[i].y][lights[i].x][dir_index] = true;
            char current_cell = layout[lights[i].y][lights[i].x];

            // Update direction or create new lights based on the cell.
            if (current_cell == '/')
            {
                if (lights[i].direction == UP)
                {
                    lights[i].direction = RIGHT;
                }
                else if (lights[i].direction == DOWN)
                {
                    lights[i].direction = LEFT;
                }
                else if (lights[i].direction == LEFT)
                {
                    lights[i].direction = DOWN;
                }
                else if (lights[i].direction == RIGHT)
                {
                    lights[i].direction = UP;
                }
            }
            else if (current_cell == '\\')
            {
                if (lights[i].direction == UP)
                {
                    lights[i].direction = LEFT;
                }
                else if (lights[i].direction == DOWN)
                {
                    lights[i].direction = RIGHT;
                }
                else if (lights[i].direction == LEFT)
                {
                    lights[i].direction = UP;
                }
                else if (lights[i].direction == RIGHT)
                {
                    lights[i].direction = DOWN;
                }
            }
            else if (current_cell == '|' && (lights[i].direction == LEFT || lights[i].direction == RIGHT))
            {
                lights[i].direction = UP;
                lights = realloc(lights, (lights_arr_size + 1) * sizeof(Light));
                lights[lights_arr_size++] = (Light){.x = lights[i].x, .y = lights[i].y, .direction = DOWN, .is_finished = false};
            }
            else if (current_cell == '-' && (lights[i].direction == UP || lights[i].direction == DOWN))
            {
                lights[i].direction = LEFT;
                lights = realloc(lights, (lights_arr_size + 1) * sizeof(Light));
                lights[lights_arr_size++] = (Light){.x = lights[i].x, .y = lights[i].y, .direction = RIGHT, .is_finished = false};
            }

            // Move the light.
            if (lights[i].direction == UP)
            {
                lights[i].y -= 1;
            }
            else if (lights[i].direction == DOWN)
            {
                lights[i].y += 1;
            }
            else if (lights[i].direction == LEFT)
            {
                lights[i].x -= 1;
            }
            else if (lights[i].direction == RIGHT)
            {
                lights[i].x += 1;
            }

            // Stop if out of bounds.
            if (lights[i].y >= LAYOUT_MAX_Y || lights[i].y < 0 || lights[i].x >= LAYOUT_MAX_X || lights[i].x < 0)
            {
                lights[i].is_finished = true;
            }
        }
    }

    // Count visited tiles.
    int count = 0;
    for (int y = 0; y < LAYOUT_MAX_Y; y++)
    {
        for (int x = 0; x < LAYOUT_MAX_X; x++)
        {
            if (visited[y][x][UP] || visited[y][x][DOWN] || visited[y][x][LEFT] || visited[y][x][RIGHT])
            {
                count++;
            }
        }
    }

    free(lights);
    return count;
}

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

    Light initial_light = (Light){.x = 0, .y = 0, .direction = RIGHT, .is_finished = false};

    return process_light(layout, initial_light);
}

// Same as part 1, but we just change the initial light and find the one that
// generates the most energized tiles.
// The starting point can be any edge tile.
int part_2(char **file_content)
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

    int max_count = 0;

    // Calculate leftmost column.
    for (int i = 0; i < LAYOUT_MAX_Y; i++)
    {
        Light initial_light = (Light){.x = 0, .y = i, .direction = RIGHT, .is_finished = false};
        int count = process_light(layout, initial_light);

        if (count > max_count)
        {
            max_count = count;
        }
    }

    // Calculate rightmost column.
    for (int i = 0; i < LAYOUT_MAX_Y; i++)
    {
        Light initial_light = (Light){.x = LAYOUT_MAX_X - 1, .y = i, .direction = LEFT, .is_finished = false};
        int count = process_light(layout, initial_light);

        if (count > max_count)
        {
            max_count = count;
        }
    }

    // Calculate top row.
    for (int i = 0; i < LAYOUT_MAX_X; i++)
    {
        Light initial_light = (Light){.x = i, .y = 0, .direction = DOWN, .is_finished = false};
        int count = process_light(layout, initial_light);

        if (count > max_count)
        {
            max_count = count;
        }
    }

    // Calculate bottom row.
    for (int i = 0; i < LAYOUT_MAX_X; i++)
    {
        Light initial_light = (Light){.x = i, .y = LAYOUT_MAX_Y - 1, .direction = UP, .is_finished = false};
        int count = process_light(layout, initial_light);

        if (count > max_count)
        {
            max_count = count;
        }
    }

    return max_count;
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
