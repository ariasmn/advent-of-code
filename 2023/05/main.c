#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#include "../helper.h"

#define SEEDS_NUMBER 20
#define RANGES_NUMBER 10

#define SEED_TO_SOIL "seed-to-soil map:"
#define SOIL_TO_FERTILIZER "soil-to-fertilizer map:"
#define FERTILIZER_TO_WATER "fertilizer-to-water map:"
#define WATER_TO_LIGHT "water-to-light map:"
#define LIGHT_TO_TEMPERATURE "light-to-temperature map:"
#define TEMPERATURE_TO_HUMIDITY "temperature-to-humidity map:"
#define HUMIDITY_TO_LOCATION "humidity-to-location map:"

enum CurrentMap
{
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

struct Range
{
    long long int start;
    long long int end;
};

enum CurrentMap get_current_map(char *current_line)
{
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

// Process a seed through all maps
void process_seed(long long int *seed, struct Map *maps[], int map_sizes[])
{
    // Process through all map stages
    for (int stage = 0; stage < 7; stage++)
    {
        for (int j = 0; j < map_sizes[stage]; j++)
        {
            struct Map current = maps[stage][j];
            if (long_long_int_between(*seed, current.source_range_start, current.source_range_start + current.range_length - 1))
            {
                // Update the seed with the corresponding destination value
                *seed = current.dest_range_start + (*seed - current.source_range_start);
                break; // Break after applying the first valid mapping in this stage
            }
        }
    }
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

    // Create arrays to hold maps for each stage
    struct Map *maps[7] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    int map_sizes[7] = {0, 0, 0, 0, 0, 0, 0};

    // Parse maps
    for (int i = 1; file_content[i] != NULL; i++)
    {
        // Skip new lines
        if (file_content[i][0] == '\n')
        {
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

        maps[current] = realloc(maps[current], (map_sizes[current] + 1) * sizeof(struct Map));
        maps[current][map_sizes[current]] = map;
        map_sizes[current]++;
    }

    // Process each seed through all maps
    for (int i = 0; i < seed_arr_size; i++)
    {
        process_seed(&seeds[i], maps, map_sizes);
    }

    long long int lowest_number = LLONG_MAX;
    for (int i = 0; i < seed_arr_size; i++)
    {
        if (seeds[i] < lowest_number)
        {
            lowest_number = seeds[i];
        }
    }

    // Cleanup
    for (int stage = 0; stage < 7; stage++)
    {
        free(maps[stage]);
    }

    return lowest_number;
}

int part_2(char **file_content)
{
    int range_arr_size = 0;
    struct Range *ranges = malloc(RANGES_NUMBER * sizeof(struct Range));
    char number_string[15] = {'\0'};

    long long int range_start = 0;
    long long int range_length = 0;

    for (int c = 0; file_content[0][c] != '\0'; c++)
    {
        if (isdigit(file_content[0][c]))
        {
            strncat(number_string, &file_content[0][c], 1);
            continue;
        }

        if (number_string[0] != '\0')
        {
            number_string[strlen(number_string)] = '\0';
            if (range_start == 0)
            {
                range_start = atoll(number_string);
                memset(number_string, 0, sizeof(number_string));
                continue;
            }

            range_length = atoll(number_string);
            memset(number_string, 0, sizeof(number_string));

            // If we get to this point, we are ready to parse the range.
            struct Range range = {range_start, range_start + range_length - 1};
            ranges[range_arr_size] = range;
            range_arr_size++;

            range_start = 0;
            range_length = 0;
        }
    }

    enum CurrentMap current = SEEDTOSOIL;

    // Create arrays to hold maps for each stage
    struct Map *maps[7] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    int map_sizes[7] = {0, 0, 0, 0, 0, 0, 0};

    // Parse maps
    for (int i = 1; file_content[i] != NULL; i++)
    {
        // Skip new lines
        if (file_content[i][0] == '\n')
        {
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

        maps[current] = realloc(maps[current], (map_sizes[current] + 1) * sizeof(struct Map));
        maps[current][map_sizes[current]] = map;
        map_sizes[current]++;
    }

    // Parse each range for each map.
    // Create new ranges that don't cover perfect ranges.
    // The idea is to work with all the possible ranges, and the just get the lowest of each range.
    for (int stage = 0; stage < 7; stage++)
    {
        // Prepare an empty array to store the new ranges that will be generated in this stage.
        int next_ranges_arr_size = 0;
        struct Range *next_ranges = malloc(sizeof(struct Range));

        // Loop over each map in the current stage, processing the ranges through the transformation map.
        for (int i = 0; i < map_sizes[stage]; i++)
        {
            // Prepare an empty array to store intermediate new ranges generated by the current map.
            int new_ranges_arr_size = 0;
            struct Range *new_ranges = malloc(sizeof(struct Range));
            struct Map current = maps[stage][i];

            // Process each existing range in the ranges array against the current map.
            for (int j = 0; j < range_arr_size; j++)
            {
                long long int seed_start = ranges[j].start;
                long long int seed_end = ranges[j].end;

                // If the current range starts before the source range of the map, a new range must be created.
                if (seed_start < MIN(seed_end, current.source_range_start))
                {
                    struct Range nr = {seed_start, MIN(seed_end, current.source_range_start)};
                    new_ranges = realloc(new_ranges, (new_ranges_arr_size + 1) * sizeof(struct Range));
                    new_ranges[new_ranges_arr_size] = nr;
                    new_ranges_arr_size++;
                }

                // If the current range extends beyond the mapped range, create another range to account for the excess.
                if (seed_end > MAX((current.source_range_start + current.range_length), seed_start))
                {
                    struct Range nr = {MAX((current.source_range_start + current.range_length), seed_start), seed_end};
                    new_ranges = realloc(new_ranges, (new_ranges_arr_size + 1) * sizeof(struct Range));
                    new_ranges[new_ranges_arr_size] = nr;
                    new_ranges_arr_size++;
                }

                // If the current range is fully within the map's source range, translate it to the destination range.
                if (MAX(seed_start, current.source_range_start) < MIN(seed_end, (current.source_range_start + current.range_length)))
                {
                    struct Range nr = {
                        current.dest_range_start + MAX(seed_start, current.source_range_start) - current.source_range_start,
                        current.dest_range_start + MIN(seed_end, (current.source_range_start + current.range_length)) - current.source_range_start};

                    next_ranges = realloc(next_ranges, (next_ranges_arr_size + 1) * sizeof(struct Range));
                    next_ranges[next_ranges_arr_size] = nr;
                    next_ranges_arr_size++;
                }
            }

            // Replace the old ranges with the newly generated ranges for this stage.
            free(ranges);
            ranges = malloc(new_ranges_arr_size * sizeof(struct Range));

            // Copy the new ranges generated by the current map into the main ranges array.
            for (int k = 0; k < new_ranges_arr_size; k++)
            {
                ranges[k] = new_ranges[k];
            }

            // Update the total number of ranges to reflect the newly generated ranges.
            range_arr_size = new_ranges_arr_size;

            // Cleanup temporary arrays.
            free(new_ranges);
            new_ranges_arr_size = 0;
        }

        // After processing all maps in this stage, append the next_ranges array to the main ranges array.
        ranges = realloc(ranges, (range_arr_size + next_ranges_arr_size) * sizeof(struct Range));

        // Copy all the ranges from next_ranges into the main ranges array.
        for (int k = 0; k < next_ranges_arr_size; k++)
        {
            ranges[range_arr_size + k] = next_ranges[k];
        }

        // Update the total number of ranges to reflect the new additions.
        range_arr_size += next_ranges_arr_size;
    }

    long long int min_location = LLONG_MAX;
    for (int i = 0; i < range_arr_size; i++)
    {
        if (ranges[i].start < min_location)
        {
            min_location = ranges[i].start;
        }
    }

    free(ranges);
    return min_location;
}

int main()
{
    FILE *fptr = fopen("input.txt", "r");
    char **file_content = read_file(fptr);

    if (file_content == NULL)
    {
        return 1;
    }

    // printf("Part 1: %lld\n", part_1(file_content));
    printf("Part 2: %lld\n", part_2(file_content));
}
