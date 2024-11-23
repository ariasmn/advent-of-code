#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "../helper.h"

int get_hash(const char *str)
{
    int hash = 0;
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (!isprint(str[i]))
        {
            continue;
        }
        hash += str[i];
        hash *= 17;
        hash %= 256;
    }

    return hash;
}

int part_1(char **file_content)
{
    char *input = strdup(file_content[0]);
    char *step = strtok(input, ",");

    int total_val = 0;

    while (step != NULL)
    {
        total_val += get_hash(step);
        step = strtok(NULL, ",");
    }

    return total_val;
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
