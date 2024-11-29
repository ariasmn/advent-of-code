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
#include "../helper.h"

#define GRID_SIZE 141

typedef struct
{
    int x;
    int y;
} Point;

// Directions: right, down, left, up
const int dx[] = {0, 1, 0, -1};
const int dy[] = {1, 0, -1, 0};

int max_length = 0;

bool is_valid_move(char **grid, int x, int y, bool **visited)
{
    if (x < 0 || x >= GRID_SIZE || y < 0 || y >= GRID_SIZE)
    {
        return false;
    }
    if (grid[x][y] == '#')
    {
        return false;
    }
    if (visited[x][y])
    {
        return false;
    }

    return true;
}

// Even though I much prefer BFS, in this specific problem, wouldn't be the best choice,
// mainly because the "never visiting the same tile" constraint would make it harder to code.
// Also, since we need the LONGEST, we'd need to explore all paths, and memory usage will be greater using BFS (right?).
void dfs(char **grid, int x, int y, bool **visited, int length, Point end, bool is_part_two)
{
    if (x == end.x && y == end.y)
    {
        if (length > max_length)
        {
            max_length = length;
        }
        return;
    }

    visited[x][y] = true;

    char current = grid[x][y];
    if (!is_part_two && (current == '>' || current == 'v' || current == '<' || current == '^'))
    {
        // Handle slopes.
        int dir;
        switch (current)
        {
        case '>':
            dir = 0;
            break;
        case 'v':
            dir = 1;
            break;
        case '<':
            dir = 2;
            break;
        case '^':
            dir = 3;
            break;
        default:
            dir = 0;
        }

        int new_x = x + dx[dir];
        int new_y = y + dy[dir];

        if (is_valid_move(grid, new_x, new_y, visited))
        {
            dfs(grid, new_x, new_y, visited, length + 1, end, is_part_two);
        }
    }
    else
    {
        // Regular path.
        for (int dir = 0; dir < 4; dir++)
        {
            int new_x = x + dx[dir];
            int new_y = y + dy[dir];

            if (is_valid_move(grid, new_x, new_y, visited))
            {
                dfs(grid, new_x, new_y, visited, length + 1, end, is_part_two);
            }
        }
    }

    visited[x][y] = false;
}

int part_1(char **file_content)
{
    // Always the same.
    Point start = {0, 1};
    Point end = {GRID_SIZE - 1, GRID_SIZE - 2};

    // Initialize visited array
    bool **visited = malloc(GRID_SIZE * sizeof(bool *));
    for (int i = 0; i < GRID_SIZE; i++)
    {
        visited[i] = calloc(GRID_SIZE, sizeof(bool));
    }

    max_length = 0;
    // It hit me now: no need to parse grids the way I've been doing it in past problems, since
    // it's already "parsed" with the read_file function...
    dfs(file_content, start.x, start.y, visited, 0, end, false);

    // Free visited array
    for (int i = 0; i < GRID_SIZE; i++)
    {
        free(visited[i]);
    }
    free(visited);

    return max_length;
}

// Literally the same as part 1, just avoiding the special treatment on the slopes when calculating.
// It takes longer since we are "bruteforcing", around 6 minutes, but couldn't care less at this point.
int part_2(char **file_content)
{
    Point start = {0, 1};
    Point end = {GRID_SIZE - 1, GRID_SIZE - 2};

    bool **visited = malloc(GRID_SIZE * sizeof(bool *));
    for (int i = 0; i < GRID_SIZE; i++)
    {
        visited[i] = calloc(GRID_SIZE, sizeof(bool));
    }

    max_length = 0;
    dfs(file_content, start.x, start.y, visited, 0, end, true);

    // Free visited array
    for (int i = 0; i < GRID_SIZE; i++)
    {
        free(visited[i]);
    }
    free(visited);

    return max_length;
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

    return 0;
}
