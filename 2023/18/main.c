#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <regex.h>

#include "../helper.h"

typedef struct
{
    int x;
    int y;
} Point;

// Function to add two points
Point point_add(Point a, Point mul_point, int meters)
{
    Point result = {
        a.x + mul_point.x * meters,
        a.y + mul_point.y * meters};
    return result;
}

// At first, I solved this reusing the day 10 algorithm, using Ray Casting (could also be done using Flood Filling).
// However, I checked on Reddit and learnt abous this Gauss' shoelace formula, which is surprisingly simple.
int part_1(char **file_content)
{
    // Direction map equivalent.
    // Used to calculate coords changes.
    struct DirMap
    {
        char dir;
        Point point;
    } dig_dir[] = {
        {'U', {0, -1}},
        {'D', {0, 1}},
        {'L', {-1, 0}},
        {'R', {1, 0}}};

    // Regex compilation (using POSIX regex).
    regex_t regex;
    int reti = regcomp(&regex, "([UDLR]) ([0-9]+) \\(#([0-9a-fA-F]+)([0-3])\\)", REG_EXTENDED);

    int area = 0;
    Point start = {0, 0};
    regmatch_t matches[5];

    for (int i = 0; file_content[i] != NULL; i++)
    {
        char *line = file_content[i];

        // Skip empty lines.
        if (line == NULL || strlen(line) == 0)
            continue;

        // Try to match regex
        if (regexec(&regex, line, 5, matches, 0) == 0)
        {
            // Extract direction.
            char direction = line[matches[1].rm_so];

            // Find matching direction point.
            Point dir_point = {0, 0};
            for (int i = 0; i < 4; i++)
            {
                if (dig_dir[i].dir == direction)
                {
                    dir_point = dig_dir[i].point;
                    break;
                }
            }

            // Extract meters.
            char meters_str[20];
            // Not a regex god, just ChatGPT helping a hopeless man.
            strncpy(meters_str, line + matches[2].rm_so, matches[2].rm_eo - matches[2].rm_so);
            meters_str[matches[2].rm_eo - matches[2].rm_so] = '\0';
            int meters = atoi(meters_str);

            // Calculate destination final point.
            // This means, each line, given the meters and the direction, we know the final Point{x,y}.
            Point next = point_add(start, dir_point, meters);

            // Shoelace formula.
            // Probably, Wikipedia might explain how this works better than me, so take a look at it.
            area += ((start.x * next.y) - (start.y * next.x) + meters);

            // Part of the algorithm: the calculated point will be the starting one in the next iteration.
            start = next;
        }
    }

    // Free regex
    regfree(&regex);

    return area / 2 + 1; // + 1 because of border width.
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
