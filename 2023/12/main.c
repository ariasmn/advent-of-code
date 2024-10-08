#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "../helper.h"

// Global pointer for the 3D memoization array.
long long ***memo;

// Function to allocate and initialize the 3D memoization array.
void ***create_memo(int max_pos, int max_group, int max_length) {
    memo = (long long ***)malloc((max_pos + 1) * sizeof(long long **));
    if (memo == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    for (int i = 0; i <= max_pos; i++) {
        memo[i] = (long long **)malloc((max_group + 1) * sizeof(long long *));
        if (memo[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }

        for (int j = 0; j <= max_group; j++) {
            memo[i][j] = (long long *)malloc((max_length + 1) * sizeof(long long));
            if (memo[i][j] == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                exit(1);
            }

            for (int k = 0; k <= max_length; k++) {
                memo[i][j][k] = -1;
            }
        }
    }
}

// Function to free the dynamically allocated memory.
void free_memo(int max_pos, int max_group) {
    for (int i = 0; i <= max_pos; i++) {
        for (int j = 0; j <= max_group; j++) {
            free(memo[i][j]);
        }
        free(memo[i]);
    }
    free(memo);
}

// Recursive function to count the total arragements per line.
long long count_arrangements(const char *spring, const int *conditions, int num_conditions, int pos, int group, int length) {
    // End of the spring
    if (spring[pos] == '\0') {
        if (group == num_conditions && length == 0) {
            return 1;
        }
        if (group == num_conditions - 1 && length == conditions[group]) {
            return 1;
        }
        return 0;
    }

    // Check if the value has already been computed
    if (memo[pos][group][length] != -1) {
        return memo[pos][group][length];
    }

    long long result = 0;

    // Operational or unknown
    if (spring[pos] == '.' || spring[pos] == '?') {
        if (length == 0) {
            result += count_arrangements(spring, conditions, num_conditions, pos + 1, group, 0);
        } else if (group < num_conditions && length == conditions[group]) {
            result += count_arrangements(spring, conditions, num_conditions, pos + 1, group + 1, 0);
        }
    }

    // Damaged or unknown
    if (spring[pos] == '#' || spring[pos] == '?') {
        if (group < num_conditions && length < conditions[group]) {
            result += count_arrangements(spring, conditions, num_conditions, pos + 1, group, length + 1);
        }
    }

    // Store the result in the memoization table
    memo[pos][group][length] = result;
    return result;
}

long long int part_1(char **file_content)
{
    long long int total_arrangements = 0;

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
        while (token != NULL)
        {
            conditions = realloc(conditions, (conditions_arr_size + 1) * sizeof(int));
            conditions[conditions_arr_size++] = atoi(token);
            token = strtok(NULL, ",");
        }

        // Compute max_pos, max_group, and max_length dynamically.
        int max_pos = strlen(spring);
        int max_group = conditions_arr_size;
        int max_length = 0;
        for (int i = 0; i < conditions_arr_size; i++)
        {
            if (conditions[i] > max_length)
            {
                max_length = conditions[i];
            }
        }

        create_memo(max_pos, max_group, max_length);

        long long arrangements = count_arrangements(spring, conditions, conditions_arr_size, 0, 0, 0);
        total_arrangements += arrangements;

        free(conditions);
        free(line);
        free_memo(max_pos, max_group);
    }

    return total_arrangements;
}

long long int part_2(char **file_content)
{
    long long total_arrangements = 0;

    for (int i = 0; file_content[i] != NULL; i++)
    {
        char *line = malloc(strlen(file_content[i]) + 1);
        strcpy(line, file_content[i]);

        // We have the spring as a string here.
        char *spring = strtok(line, " ");
        char *conditions_as_string = strtok(NULL, " ");
        // Remove new line
        conditions_as_string[strlen(conditions_as_string) - 1] = 0;

        // Buffers to duplicate
        size_t spring_len = strlen(spring);
        size_t conditions_len = strlen(conditions_as_string);
        // 5 copies + 5 separators + 1 for null terminator
        char *buffer_spring = malloc((spring_len * 5) + 6);
        char *buffer_conditions = malloc((conditions_len * 5) + 6);
        snprintf(buffer_spring, (spring_len * 5) + 6, "%s?%s?%s?%s?%s", spring, spring, spring, spring, spring);
        snprintf(buffer_conditions, (conditions_len * 5) + 6, "%s,%s,%s,%s,%s", conditions_as_string, conditions_as_string, conditions_as_string, conditions_as_string, conditions_as_string);

        int *conditions = calloc(1, sizeof(int));
        int conditions_arr_size = 0;

        char *token = strtok(buffer_conditions, ",");
        while (token != NULL)
        {
            conditions = realloc(conditions, (conditions_arr_size + 1) * sizeof(int));
            conditions[conditions_arr_size++] = atoi(token);
            token = strtok(NULL, ",");
        }

        // Compute max_pos, max_group, and max_length dynamically.
        int max_pos = strlen(buffer_spring);
        int max_group = conditions_arr_size;
        int max_length = 0;
        for (int i = 0; i < conditions_arr_size; i++)
        {
            if (conditions[i] > max_length)
            {
                max_length = conditions[i];
            }
        }

        create_memo(max_pos, max_group, max_length);

        long long arrangements = count_arrangements(buffer_spring, conditions, conditions_arr_size, 0, 0, 0);
        total_arrangements += arrangements;

        free(buffer_spring);
        free(buffer_conditions);
        free(conditions);
        free(line);
        free_memo(max_pos, max_group);
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

    printf("Part 1: %lld\n", part_1(file_content));
    printf("Part 2: %lld\n", part_2(file_content));
}