#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#include "../helper.h"

#define SEEDS_NUMBER 20

#define SEED_TO_SOIL "seed-to-soil map:"
#define SOIL_TO_FERTILIZER "soil-to-fertilizer map:"
#define FERTILIZER_TO_WATER "fertilizer-to-water map:"
#define WATER_TO_LIGHT "water-to-light map:"
#define LIGHT_TO_TEMPERATURE "light-to-temperature map:"
#define TEMPERATURE_TO_HUMIDITY "temperature-to-humidity map:"
#define HUMIDITY_TO_LOCATION "humidity-to-location map:"

enum CurrentMap {
    SEEDTOSOIL,
    SOILTOFERTILIZER,
    FERTILIZERTOWATER,
    WATERTOLIGHT,
    LIGHTTOTEMPERATURE,
    TEMPERATURETOHUMIDITY,
    HUMIDITYTOLOCATION
};

struct Map
{
    long long int dest_range_start;
    long long int source_range_start;
    long long int range_length;
};

enum CurrentMap get_current_map(char *current_line) {
    // Remove new line before compare.
    char *without_new_line = str_replace(current_line, "\n", "");

    if (string_equal(without_new_line, SEED_TO_SOIL))
    {
        return SEEDTOSOIL;
    }

    if (string_equal(without_new_line, SOIL_TO_FERTILIZER))
    {
        return SOILTOFERTILIZER;
    }

    if (string_equal(without_new_line, FERTILIZER_TO_WATER))
    {
        return FERTILIZERTOWATER;
    }
    
    if (string_equal(without_new_line, WATER_TO_LIGHT))
    {
        return WATERTOLIGHT;
    }

    if (string_equal(without_new_line, LIGHT_TO_TEMPERATURE))
    {
        return LIGHTTOTEMPERATURE;
    }

    if (string_equal(without_new_line, TEMPERATURE_TO_HUMIDITY))
    {
        return TEMPERATURETOHUMIDITY;
    }

    return HUMIDITYTOLOCATION;
}

int part_1(char **file_content)
{
    int seed_arr_size = 0;
    long long int seeds[SEEDS_NUMBER] = {0};
    char number_string[15] = {'\0'};

    // Parse the first line manually, which is the seeds line.
    for (int c = 0; file_content[0][c] != '\0'; c++)
    {
        if (isdigit(file_content[0][c]))
        {
            strncat(number_string, &file_content[0][c], 1);
            continue;
        }

        if (number_string[0] != '\0')
        {
            // Terminate the string so "atoll" works.
            number_string[strlen(number_string)] = '\0';
            seeds[seed_arr_size] = atoll(number_string);
            seed_arr_size++;

            memset(number_string, 0, sizeof(number_string));
        }
    }

    enum CurrentMap current = SEEDTOSOIL;

    int seed_to_soil_arr_size = 0;
    struct Map *seed_to_soil = NULL;

    int soil_to_fertilizer_arr_size = 0;
    struct Map *soil_to_fertilizer = NULL;

    int fertilizer_to_water_arr_size = 0;
    struct Map *fertilizer_to_water = NULL;

    int water_to_light_arr_size = 0;
    struct Map *water_to_light = NULL;

    int light_to_temperature_arr_size = 0;
    struct Map *light_to_temperature = NULL;

    int temperature_to_humidity_arr_size = 0;
    struct Map *temperature_to_humidity = NULL;

    int humidity_to_location_arr_size = 0;
    struct Map *humidity_to_location = NULL;

    // Parse maps
    for (int i = 1; file_content[i] != NULL; i++)
    {
        // Skip new lines
        if (file_content[i][0] == '\n') {
            continue;
        }

        // Set the current map
        if (!isdigit(file_content[i][0]))
        {
            current = get_current_map(file_content[i]);
            continue;
        }

        // If we get to this point, the line needs to be parsed as a map.
        struct Map map;
        sscanf(file_content[i], "%lld %lld %lld", &map.dest_range_start, &map.source_range_start, &map.range_length);

        // Add to the correct map.
        switch (current)
        {
            case SEEDTOSOIL:
                seed_to_soil = realloc(seed_to_soil, (seed_to_soil_arr_size + 1) * sizeof(struct Map));
                seed_to_soil[seed_to_soil_arr_size] = map;
                seed_to_soil_arr_size++;
                break;
            case SOILTOFERTILIZER:
                soil_to_fertilizer = realloc(soil_to_fertilizer, (soil_to_fertilizer_arr_size + 1) * sizeof(struct Map));
                soil_to_fertilizer[soil_to_fertilizer_arr_size] = map;
                soil_to_fertilizer_arr_size++;
                break;
            case FERTILIZERTOWATER:
                fertilizer_to_water = realloc(fertilizer_to_water, (fertilizer_to_water_arr_size + 1) * sizeof(struct Map));
                fertilizer_to_water[fertilizer_to_water_arr_size] = map;
                fertilizer_to_water_arr_size++;
                break;
            case WATERTOLIGHT:
                water_to_light = realloc(water_to_light, (water_to_light_arr_size + 1) * sizeof(struct Map));
                water_to_light[water_to_light_arr_size] = map;
                water_to_light_arr_size++;
                break;
            case LIGHTTOTEMPERATURE:
                light_to_temperature = realloc(light_to_temperature, (light_to_temperature_arr_size + 1) * sizeof(struct Map));
                light_to_temperature[light_to_temperature_arr_size] = map;
                light_to_temperature_arr_size++;
                break;
            case TEMPERATURETOHUMIDITY:
                temperature_to_humidity = realloc(temperature_to_humidity, (temperature_to_humidity_arr_size + 1) * sizeof(struct Map));
                temperature_to_humidity[temperature_to_humidity_arr_size] = map;
                temperature_to_humidity_arr_size++;
                break;
            case HUMIDITYTOLOCATION:
                humidity_to_location = realloc(humidity_to_location, (humidity_to_location_arr_size + 1) * sizeof(struct Map));
                humidity_to_location[humidity_to_location_arr_size] = map;
                humidity_to_location_arr_size++;
                break;
        }
    }

    // With everything parsed, we start calculate.
    // Iterate each number and do the substitutions.
    // This is extremely rudimentary, but I'm really hating this day.
    for (int i = 0; i < seed_arr_size; i++)
    {
        for (int j = 0; j < seed_to_soil_arr_size; j++)
        {
            struct Map current = seed_to_soil[j];
            if (long_long_int_between(seeds[i], current.source_range_start, (current.source_range_start + current.range_length - 1)))
            {
                seeds[i] = current.dest_range_start + (seeds[i] - current.source_range_start);
                break;
            }
        }

        for (int j = 0; j < soil_to_fertilizer_arr_size; j++)
        {
            struct Map current = soil_to_fertilizer[j];
            if (long_long_int_between(seeds[i], current.source_range_start, (current.source_range_start + current.range_length - 1)))
            {
                seeds[i] = current.dest_range_start + (seeds[i] - current.source_range_start);
                break;
            }
        }

        for (int j = 0; j < fertilizer_to_water_arr_size; j++)
        {
            struct Map current = fertilizer_to_water[j];
            if (long_long_int_between(seeds[i], current.source_range_start, (current.source_range_start + current.range_length - 1)))
            {
                seeds[i] = current.dest_range_start + (seeds[i] - current.source_range_start);
                break;
            }
        }

        for (int j = 0; j < water_to_light_arr_size; j++)
        {
            struct Map current = water_to_light[j];
            if (long_long_int_between(seeds[i], current.source_range_start, (current.source_range_start + current.range_length - 1)))
            {
                seeds[i] = current.dest_range_start + (seeds[i] - current.source_range_start);
                break;
            }
        }

        for (int j = 0; j < light_to_temperature_arr_size; j++)
        {
            struct Map current = light_to_temperature[j];
            if (long_long_int_between(seeds[i], current.source_range_start, (current.source_range_start + current.range_length - 1)))
            {
                seeds[i] = current.dest_range_start + (seeds[i] - current.source_range_start);
                break;
            }
        }

        for (int j = 0; j < temperature_to_humidity_arr_size; j++)
        {
            struct Map current = temperature_to_humidity[j];
            if (long_long_int_between(seeds[i], current.source_range_start, (current.source_range_start + current.range_length - 1)))
            {
                seeds[i] = current.dest_range_start + (seeds[i] - current.source_range_start);
                break;
            }
        }

        for (int j = 0; j < humidity_to_location_arr_size; j++)
        {
            struct Map current = humidity_to_location[j];
            if (long_long_int_between(seeds[i], current.source_range_start, (current.source_range_start + current.range_length - 1)))
            {
                seeds[i] = current.dest_range_start + (seeds[i] - current.source_range_start);
                break;
            }
        }
    }

    long long int lowest_number = LLONG_MAX;
    for (int i = 0; i < seed_arr_size; i++)
    {
        if (seeds[i] < lowest_number)
        {
            lowest_number = seeds[i];
        }
    }

    return lowest_number;
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
}
