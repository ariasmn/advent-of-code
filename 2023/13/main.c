#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "../helper.h"

int current_part = 1;

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

int count_differences(const char *str1, const char *str2)
{
    int differences = 0;
    while (*str1 && *str2)
    {
        if (*str1 != *str2)
        {
            differences++;
        }
        str1++;
        str2++;
    }
    // If strings are of different lengths, count remaining characters as differences
    while (*str1)
    {
        differences++;
        str1++;
    }
    while (*str2)
    {
        differences++;
        str2++;
    }
    return differences;
}

int find_reflection(char **pattern, int rows)
{
    for (int row = 1; row < rows; row++)
    {
        int total_differences = 0;
        bool is_reflection = true;

        for (int i = 0; row - i - 1 >= 0 && row + i < rows; i++)
        {
            int before = row - i - 1;
            int after = row + i;

            // I know I know this isn't ideal but I was lazy to refactor it in a better way.
            if (current_part == 1)
            {
                // No row to compare, so if we get to this point, it's an horizontal reflection.
                if (strcmp(pattern[after], pattern[before]) != 0)
                {
                    is_reflection = false;
                    break;
                }
            }
            else
            {
                // If the total differences is just 1 (or 0), we need to count it as a reflection.
                // It's in reality, the same as the part_1 approach, just checking that there might be a single difference
                // instead of none.
                int diffs = count_differences(pattern[after], pattern[before]);
                total_differences += diffs;
                if (total_differences > 1)
                {
                    is_reflection = false;
                    break;
                }
            }
        }

        if (is_reflection && (current_part == 1 || total_differences == 1))
        {
            return row;
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

int part_2(char **file_content)
{
    current_part = 2;

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
    printf("Part 2: %d\n", part_2(file_content));
}