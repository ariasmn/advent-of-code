#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "../helper.h"

#define NUMBER_RACES 4

struct Race {
    int time;
    int distance;
};

int part_1(char **file_content)
{
    // We only have two lines here, so no need to iterate the file.
    int races_arr_size = 0;
    struct Race races[NUMBER_RACES];
    char number_string[5] = {'\0'};

    // Get the time.
    for (int i = 0; file_content[0][i] != '\0'; i++)
    {
        if (isdigit(file_content[0][i]))
        {
            strncat(number_string, &file_content[0][i], 1);
            continue;
        }

        if (number_string[0] != '\0')
        {
            number_string[strlen(number_string)] = '\0';
            struct Race nr = {atoi(number_string)};

            races[races_arr_size] = nr;
            races_arr_size++;

            memset(number_string, 0, sizeof(number_string));
        }
    }

    // Get the distance.
    int current_race = 0;
    for (int i = 0; file_content[1][i] != '\0'; i++)
    {
        if (isdigit(file_content[1][i]))
        {
            strncat(number_string, &file_content[1][i], 1);
            continue;
        }

        if (number_string[0] != '\0')
        {
            number_string[strlen(number_string)] = '\0';
            races[current_race].distance = atoi(number_string);
            current_race++;

            memset(number_string, 0, sizeof(number_string));
        }
    }

    int total_ways = 1;
    for (int i = 0; i < races_arr_size; i++)
    {
        int race_ways_count = 0;
        for (int j = 0; j < races[i].time; j++)
        {
            if (j * (races[i].time - j) > races[i].distance)
            {
                race_ways_count++;
            }
        }
        total_ways *= race_ways_count;
    }

    return total_ways;
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
    // printf("Part 2: %lld\n", part_2(file_content));
}
