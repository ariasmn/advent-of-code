// DISCLAIMER: Starting day 21, I've been just looking at solutions from another languages I know (Golang, PHP...)
// and basically doing it in my own way in C.
// I looked at different solutions and took ideas from all of them to make my own version, so I can't link to the solution directly,
// but all of the solutions can be found in the subreddit: https://www.reddit.com/r/adventofcode/.
// When I started AoC 2023, it was because I wanted to learn C and
// honestly, I wasn't in the mood to solve certain puzzles (I got a bit tired of it in AoC 2022).
// So, since I struggled A LOT in day 20, I've decided to just do it this way.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "../helper.h"

#define MIN_BOUND 200000000000000.0L
#define MAX_BOUND 400000000000000.0L

typedef struct
{
    long double px, py, pz; // Position coordinates in 3D space.
    long double vx, vy, vz; // Velocity components in 3D space.
} Hailstone;

// The idea is that if two hailstones intersect each other, their paths must cross at some point.
// We can find this intersection by:
// 1. Converting each hailstone's path into a line equation (y = mx + b)
// 2. Finding where these lines intersect by solving the system of equations
// 3. Checking if this intersection point:
//    - Is reached by moving forward in time for both hailstones
//    - Falls within our target area bounds.
bool check_intersection(Hailstone a, Hailstone b, long double *x, long double *y, long double min_bound, long double max_bound)
{
    // Convert to straight line equation (y = mx + b).
    // Slope m is calculated as change in y over change in x (dy/dx).
    long double m1 = a.vy / a.vx;
    long double m2 = b.vy / b.vx;
    
    // Calculate y-intercepts using point-slope form: b = y - mx.
    long double b1 = a.py - m1 * a.px;
    long double b2 = b.py - m2 * b.px;

    // If slopes are equal, lines are parallel and never intersect.
    // Using this to check for floating point imprecision.
    if (fabsl(m1 - m2) < 1e-10L)
    {
        return false;
    }

    // Find intersection point by solving system of equations:
    // y = m1x + b1
    // y = m2x + b2
    // Therefore: m1x + b1 = m2x + b2
    // x = (b2 - b1)/(m1 - m2).
    *x = (b2 - b1) / (m1 - m2);
    *y = m1 * (*x) + b1;

    // Check if intersection happens in the future by verifying
    // if we need to move in the same direction as velocity to reach intersection.
    // This is because we just need to check if they intersect given the starting point, and not before that starting point.
    bool a_future = ((*x - a.px) / a.vx >= 0);
    bool b_future = ((*x - b.px) / b.vx >= 0);

    if (!a_future || !b_future)
    {
        return false;
    }

    // Check if intersection point lies within the test area bounds.
    return *x >= min_bound && *x <= max_bound &&
           *y >= min_bound && *y <= max_bound;
}

int part_1(char **file_content)
{
    int hailstones_arr_size = 0;
    Hailstone *hailstones = NULL;

    for (int i = 0; file_content[i] != NULL; i++)
    {
        hailstones = realloc(hailstones, (hailstones_arr_size + 1) * sizeof(Hailstone));
        
        double px, py, pz, vx, vy, vz;
        sscanf(file_content[i], "%lf, %lf, %lf @ %lf, %lf, %lf",
               &px, &py, &pz, &vx, &vy, &vz);

        // Convert parsed doubles to long doubles for better precision.
        hailstones[hailstones_arr_size].px = (long double)px;
        hailstones[hailstones_arr_size].py = (long double)py;
        hailstones[hailstones_arr_size].pz = (long double)pz;
        hailstones[hailstones_arr_size].vx = (long double)vx;
        hailstones[hailstones_arr_size].vy = (long double)vy;
        hailstones[hailstones_arr_size].vz = (long double)vz;
        hailstones_arr_size++;
    }

    int intersections = 0;
    long double x, y;

    for (int i = 0; i < hailstones_arr_size; i++)
    {
        for (int j = i + 1; j < hailstones_arr_size; j++)
        {
            if (check_intersection(hailstones[i], hailstones[j], &x, &y, MIN_BOUND, MAX_BOUND))
            {
                intersections++;
            }
        }
    }

    free(hailstones);
    return intersections;
}

long long int part_2(char **file_content)
{
    return 0;
}

int main(int argc, char *argv[])
{
    FILE *fptr = fopen("input.txt", "r");
    char **file_content = read_file(fptr);

    if (file_content == NULL)
    {
        return 1;
    }

    printf("Part 1: %d\n", part_1(file_content));
    printf("Part 2: %lld\n", part_2(file_content));

    return 0;
}
