#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "../helper.h"

struct Galaxy
{
    int x;
    int y;
};

void expand_galaxies(struct Galaxy galaxies[], int size)
{
    // Find max x and y
    int max_x = 0, max_y = 0;
    for (int i = 0; i < size; i++)
    {
        if (galaxies[i].x > max_x)
            max_x = galaxies[i].x;
        if (galaxies[i].y > max_y)
            max_y = galaxies[i].y;
    }

    // Allocate and initialize arrays
    int *shift_x = calloc(max_x + 1, sizeof(int));
    int *shift_y = calloc(max_y + 1, sizeof(int));

    // Mark present values
    for (int i = 0; i < size; i++)
    {
        shift_x[galaxies[i].x] = 1;
        shift_y[galaxies[i].y] = 1;
    }

    // Calculate cumulative shifts
    int count_x = 0, count_ = 0;
    for (int i = 0; i <= max_x; i++)
    {
        if (shift_x[i] == 0)
        {
            count_x++;
        }
        shift_x[i] = count_x;
    }
    for (int i = 0; i <= max_y; i++)
    {
        if (shift_y[i] == 0)
        {
            count_++;
        }
        shift_y[i] = count_;
    }

    // Update coordinates
    for (int i = 0; i < size; i++)
    {
        galaxies[i].x += shift_x[galaxies[i].x];
        galaxies[i].y += shift_y[galaxies[i].y];
    }

    free(shift_x);
    free(shift_y);
}

int part_1(char **file_content)
{
    int galaxies_arr_size = 0;
    struct Galaxy *galaxies = malloc(1 * sizeof(struct Galaxy));

    // We first save the position of the galaxies.
    for (int i = 0; file_content[i] != NULL; i++)
    {
        for (int j = 0; file_content[i][j] != '\0'; j++)
        {
            if (file_content[i][j] == '#')
            {
                struct Galaxy galaxy = {.x = j, .y = i};
                galaxies = realloc(galaxies, (galaxies_arr_size + 1) * sizeof(struct Galaxy));
                galaxies[galaxies_arr_size] = galaxy;
                galaxies_arr_size++;
            }
        }
    }

    // We expand the galaxies, meaning, incrementing the x/y values when needed.
    expand_galaxies(galaxies, galaxies_arr_size);

    // Now, calculate the distance between pairs.
    int total_distance = 0;
    // Array size minus 1, since all the pairs will be calculated for the last one already.
    for (int i = 0; i < galaxies_arr_size - 1; i++)
    {
        for (int j = i + 1; j < galaxies_arr_size; j++)
        {
            int distance = abs(galaxies[i].x - galaxies[j].x) + abs(galaxies[i].y - galaxies[j].y);
            total_distance += distance;
        }
    }

    free(galaxies);
    return total_distance;
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