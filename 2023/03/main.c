#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "../helper.h"

struct Coordinate
{
    int x;
    int y;
};

struct Number
{
    int value;
    struct Coordinate start_coord;
    struct Coordinate end_coord;
};

int are_neighbors(struct Coordinate a, struct Coordinate b)
{
    return (abs(a.x - b.x) <= 1 && abs(a.y - b.y) <= 1);
}

int part_1(char **file_content)
{

    int number_arr_size = 0;
    struct Number *numbers = NULL;

    int symbols_arr_size = 0;
    struct Coordinate *symbols = NULL;

    for (int i = 0; file_content[i] != NULL; i++)
    {
        // Iterate each line, skipping the dots, parsing the digits and its coordinates
        // and saving the symbols coordinates.
        char number_string[50];
        number_string[0] = '\0';

        for (int j = 0; file_content[i][j] != '\0'; j++)
        {
            struct Coordinate start_coord;
            struct Coordinate end_coord;

            if (isdigit(file_content[i][j]))
            {
                if (number_string[0] == '\0')
                {
                    start_coord.x = j;
                    start_coord.y = i;
                }

                number_string[strlen(number_string)] = file_content[i][j];

                continue;
            }

            if (number_string[0] != '\0')
            {
                // Terminate the string so "atoi" works.
                number_string[strlen(number_string)] = '\0';
                // Since the number is being parsed in a dot/symbol iteration, we need to substract the position/
                end_coord.x = j -1;
                end_coord.y = i;

                // Reallocate the array before trying to append.
                numbers = realloc(numbers, (number_arr_size + 1) * sizeof(struct Number));

                numbers[number_arr_size].value = atoi(number_string);
                numbers[number_arr_size].start_coord = start_coord;
                numbers[number_arr_size].end_coord = end_coord;

                start_coord = (struct Coordinate){0};
                end_coord = (struct Coordinate){0};

                // Clear the number_string variable and increase the array size.
                memset(number_string, 0, sizeof(number_string));
                number_arr_size++;
            }

            if (file_content[i][j] == '.' || file_content[i][j] == '\n')
            {
                continue;
            }

            // If we get to this point, is a symbol, and we need to save its position.
            // Reallocate the array before trying to append.
            symbols = realloc(symbols, (symbols_arr_size + 1) * sizeof(struct Coordinate));
            symbols[symbols_arr_size].x = j;
            symbols[symbols_arr_size].y = i;
            symbols_arr_size++;
        }
    }

    // Iterate the numbers and check if there's a neighbour sybmol.
    int total_sum = 0;
    for (int i = 0; i < number_arr_size; i++)
    {
        for (int j = 0; j < symbols_arr_size; j++)
        {
            if (are_neighbors(symbols[j], numbers[i].start_coord))
            {
                total_sum += numbers[i].value;
                break;
            }
            // If not, check end coord.
            if (are_neighbors(symbols[j], numbers[i].end_coord))
            {
                total_sum += numbers[i].value;
                break;
            }
        }
    }

    return total_sum;
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
}
