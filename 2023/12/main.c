#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "../helper.h"

// Recursive function to count the total arragements per line.
long long count_arrangements(const char* spring, const int* conditions, int num_conditions, int pos, int group, int length) {
    // End of the spring
    if (spring[pos] == '\0') {
        // If we've used all groups and aren't in the middle of a group, it's valid
        if (group == num_conditions && length == 0)
        {
            return 1;
        }
        // If we're on the last group and its length matches, it's valid
        if (group == num_conditions - 1 && length == conditions[group])
        {
            return 1;
        }
        // Otherwise, it's not a valid arrangement
        return 0;
    }

    long long result = 0;

    // If the current position is operational or unknown
    if (spring[pos] == '.' || spring[pos] == '?') {
        if (length == 0) {
            // Continue to next position, staying in the same group
            result += count_arrangements(spring, conditions, num_conditions, pos + 1, group, 0);
        } else if (group < num_conditions && length == conditions[group]) {
            // We've completed a group, move to the next group
            result += count_arrangements(spring, conditions, num_conditions, pos + 1, group + 1, 0);
        }
    }

    // If the current position is damaged or unknown
    if (spring[pos] == '#' || spring[pos] == '?') {
        if (group < num_conditions && length < conditions[group]) {
            // Continue or start a group of damaged springs
            result += count_arrangements(spring, conditions, num_conditions, pos + 1, group, length + 1);
        }
    }

    return result;
}

int part_1(char **file_content)
{
    long long total_arrangements = 0;

    for (int i = 0; file_content[i] != NULL; i++)
    {
        char *line = malloc(strlen(file_content[i]) + 1);
        strcpy(line, file_content[i]);

        // We have the spring as a string here.
        char *spring = strtok(line, " ");
        char *conditions_as_string = strtok(NULL, " ");

        int *conditions = calloc(1, sizeof(int));
        int conditions_arr_size = 0;

        // Parse the conditions to an int array.
        char *token = strtok(conditions_as_string, ",");
        while (token != NULL) {
            conditions = realloc(conditions, (conditions_arr_size + 1) * sizeof(int));
            conditions[conditions_arr_size++] = atoi(token);
            token = strtok(NULL, ",");
        }

        // Count arrangements
        long long arrangements = count_arrangements(spring, conditions, conditions_arr_size, 0, 0, 0);
        total_arrangements += arrangements;

        free(conditions);
        free(line);
    }

    return total_arrangements;
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
    // printf("Part 2: %ld\n", part_2(file_content));
}