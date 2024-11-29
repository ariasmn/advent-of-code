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
void dfs(char **grid, int x, int y, bool **visited, int length, Point end)
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
    if (current == '>' || current == 'v' || current == '<' || current == '^')
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
            dfs(grid, new_x, new_y, visited, length + 1, end);
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
                dfs(grid, new_x, new_y, visited, length + 1, end);
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
    dfs(file_content, start.x, start.y, visited, 0, end);

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
    // printf("Part 2: %d\n", part_2(file_content));

    return 0;
}
