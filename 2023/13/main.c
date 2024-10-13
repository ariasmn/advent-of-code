#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "../helper.h"


char **transpose(char **pattern, int rows, int cols)
{
    // Allocate memory for the transposed result
    char **result = malloc(cols * sizeof(char *));
    for (int i = 0; i < cols; i++)
    {
        result[i] = malloc((rows + 1) * sizeof(char)); // +1 for null terminator
    }

    // Initialize the result array by transposing
    for (int i = 0; i < cols; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            result[i][j] = pattern[j][i];
        }
    }

    return result;
}

int find_reflection(char **pattern, int rows)
{
    for (int row = 1; row < rows; row++)
    {
        // If the line we are reading is the same as the previous one, we find a potential reflection.
        if (strcmp(pattern[row], pattern[row - 1]) == 0)
        {
            // Assume it's a reflection and start checking
            bool is_reflection = true;

            // We start on the next line after we find the first reflection.
            for (int i = 1; i + row < rows; i++)
            {
                int after = row + i;
                int before = row - i - 1;

                // No row to compare, so if we get to this point, it's an horizontal reflection.
                if (before < 0)
                {
                    return row;
                }

                // If any row doesn't match, it's not a valid reflection
                if (strcmp(pattern[after], pattern[before]) != 0)
                {
                    is_reflection = false;
                    break;
                }
            }

            // Only return if the reflection is confirmed
            if (is_reflection)
            {
                return row;
            }
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
            total_result += (find_reflection(pattern, pattern_rows) * 100);

            pattern = transpose(pattern, pattern_rows, pattern_cols);
            total_result += find_reflection(pattern, pattern_cols);

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