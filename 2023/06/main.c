#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "../helper.h"

#define NUMBER_RACES 4

struct Race
{
    long int time;
    long int distance;
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
            struct Race nr = {atol(number_string)};

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
            races[current_race].distance = atol(number_string);
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

int part_2(char **file_content)
{
    // We only have two lines here, so no need to iterate the file.
    int races_arr_size = 0;
    struct Race race;
    char number_string[5] = {'\0'};

    // Get the time.
    char *time_line = malloc(strlen(file_content[0]));
    strcpy(time_line, file_content[0]);
    time_line = str_replace(time_line, " ", "");
    char time_buffer[6];
    snprintf(time_buffer, sizeof(time_buffer), "Time:");
    time_line = str_replace(time_line, time_buffer, "");

    // Get the distance.
    char *distance_line = malloc(strlen(file_content[1]));
    strcpy(distance_line, file_content[1]);
    distance_line = str_replace(distance_line, " ", "");
    char distance_buffer[10];
    snprintf(distance_buffer, sizeof(distance_buffer), "Distance:");
    distance_line = str_replace(distance_line, distance_buffer, "");

    // Set the race conditions.
    race.time = atol(time_line);
    race.distance = atol(distance_line);

    // Cleanup.
    free(time_line);
    free(distance_line);

    int race_ways_count = 0;
    for (int j = 0; j < race.time; j++)
    {
        if (j * (race.time - j) > race.distance)
        {
            race_ways_count++;
        }
    }

    return race_ways_count;
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
