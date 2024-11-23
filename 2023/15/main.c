#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "../helper.h"

int part_1(char **file_content)
{
    int current_val = 0;
    int total_val = 0;

    // Is just a single line, so we need to iterate it, but not the file itself.
    for (int i = 0; file_content[0][i] != '\0'; i++)
    {
        if (file_content[0][i] == ',' || !isprint(file_content[0][i]))
        {
            total_val += current_val;
            current_val = 0;
            continue;
        }

        // Implicit cast and we get the ASCII code.
        current_val += file_content[0][i];
        current_val *= 17;
        current_val %= 256;
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
