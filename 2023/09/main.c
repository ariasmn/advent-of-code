#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "../helper.h"

#define NUMBERS_PER_LINE 21

// Checks if all the elements in an array are '0'.
bool contains_nonzero(long int *arr, int len) {
    for (int i = 0; i < len; i++) {
        if (arr[i] != 0) {
            return true;
        }
    }
    return false;
}

// Recursive function to calculate the sum results.
// It stops when all the values received are '0'.
int get_next(long int *v, int len) {
    long int *val = malloc((len - 1) * sizeof(long int));
    for (int i = 0; i < len - 1; i++) {
        val[i] = v[i + 1] - v[i];
    }

    long int result;
    if (!contains_nonzero(val, len - 1)) {
        result = v[len - 1];
    } else {
        result = v[len - 1] + get_next(val, len - 1);
    }

    free(val);
    return result;
}

long int part_1(char **file_content)
{
    long int result = 0;

    for (int i = 0; file_content[i] != NULL; i++)
    {
        long int history[NUMBERS_PER_LINE];
        int history_arr_index = 0;

        // Parse each history.
        char *token = strtok(file_content[i], " ");
        while (token != NULL)
        {
            history[history_arr_index] = atol(token);
            history_arr_index++;
            token = strtok(NULL, " ");
        }

        result += get_next(history, NUMBERS_PER_LINE);
    }

    return result;
}

int main()
{
    FILE *fptr = fopen("input.txt", "r");
    char **file_content = read_file(fptr);

    if (file_content == NULL)
    {
        return 1;
    }

    printf("Part 1: %ld\n", part_1(file_content));
    // printf("Part 2: %ld\n", part_2(file_content));
}
