#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "../helper.h"

bool is_vertical_reflection(char **pattern, int rows, int cols, int line)
{
    for (int row = 0; row < rows; row++)
    {
        for (int offset = 0; offset < cols; offset++)
        {
            int left = line - offset - 1;
            int right = line + offset;
            if (left >= 0 && right < cols)
            {
                if (pattern[row][left] != pattern[row][right])
                {
                    return false;
                }
            }
        }
    }
    return true;
}

bool is_horizontal_reflection(char **pattern, int rows, int cols, int line)
{
    // If the line we are reading is the same as the previous one, we find a reflection.
    if (strcmp(pattern[line], pattern[line - 1]) == 0)
    {
        // We start on the next line after we find the first reflection.
        for (int i = 1; i + line < rows; i++)
        {
            int after = line + i;
            int before = line - i - 1;

            // No row to compare, so if we get to this point, it's an horizontal reflection.
            // We only need to check the before since the after is contemplated in the for loop conditions.
            if (before < 0)
            {
                return true;
            }

            if (strcmp(pattern[after], pattern[before]) != 0)
            {
                return false;
            }
        }

        return true;
    }

    return false;
}

int find_reflection(char **pattern, int rows, int cols)
{
    // We start at one, since there can't be no reflection in the first position since it's out of bounds.
    // This applies for both the vertical and horizontal.
    for (int col = 1; col < cols; col++)
    {
        if (is_vertical_reflection(pattern, rows, cols, col))
        {
            return col;
        }
    }

    for (int row = 1; row < rows; row++)
    {
        if (is_horizontal_reflection(pattern, rows, cols, row))
        {
            return 100 * row;
        }
    }

    return 0; // No reflection found
}

int part_1(char **file_content)
{
    int total_result = 0;
    int pattern_rows = 0;
    int pattern_cols = 0;
    char **pattern = NULL;

    for (int i = 0; file_content[i] != NULL; i++)
    {
        // Pattern ending, either by new line or by end of file.
        // Calculate reflections and clear the pattern.
        if (file_content[i][0] == '\n' || file_content[i + 1] == NULL)
        {
            if (file_content[i][0] != '\n')
            {
                pattern = realloc(pattern, (pattern_rows + 1) * sizeof(char *));
                pattern[pattern_rows] = file_content[i];
                pattern_rows++;
            }

            pattern_cols = strlen(pattern[0]) - 1;
            total_result += find_reflection(pattern, pattern_rows, pattern_cols);

            pattern = NULL;
            pattern_rows = 0;
            pattern_cols = 0;

            continue;
        }

        // Parse the pattern each line.
        pattern = realloc(pattern, (pattern_rows + 1) * sizeof(char *));
        pattern[pattern_rows] = file_content[i];
        pattern_rows++;
    }

    free(pattern);
    return total_result;
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
    // printf("Part 2: %lld\n", part_2(file_content));
}