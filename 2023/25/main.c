// DISCLAIMER: Starting day 21, I've been just looking at solutions from another languages I know (Golang, PHP...)
// and basically doing it in my own way in C.
// I looked at different solutions and took ideas from all of them to make my own version, so I can't link to the solution directly,
// but all of the solutions can be found in the subreddit: https://www.reddit.com/r/adventofcode/.
// When I started AoC 2023, it was because I wanted to learn C and
// honestly, I wasn't in the mood to solve certain puzzles (I got a bit tired of it in AoC 2022).
// So, since I struggled A LOT in day 20, I've decided to just do it this way.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "../helper.h"

#define MAX_NODES 2500
#define MAX_NAME 20
#define MAX_EDGES 20000

typedef struct
{
    int from;
    int to;
} Edge;

int get_vertex_id(char *name, char names[][MAX_NAME], int *count)
{
    for (int i = 0; i < *count; i++)
    {
        if (strcmp(names[i], name) == 0)
            return i;
    }

    strcpy(names[*count], name);
    return (*count)++;
}

// We need to find exactly 3 wires to disconnect that will split
// the component network into exactly 2 groups. This is essentially asking us
// to find a "minimum cut" of size 3 in the graph.
//
// Why Karger's algorithm is perfect for this:
// 1. We KNOW there exists a cut of size 3 (the puzzle guarantees this)
// 2. This cut splits the graph into exactly 2 components (as required)
// 3. Any other cut in the graph must be larger than 3 (or the answer wouldn't be unique)
//
// These properties make Karger's particularly effective because:
// - When it randomly contracts edges, it's less likely to contract one of the 3 critical edges
//   (since there are only 3 of them) than other edges
// - If we avoid contracting those 3 edges, we'll naturally find our answer
// - The algorithm will eventually "get lucky" and keep those 3 edges separate
int part_1(char **file_content)
{
    char names[MAX_NODES][MAX_NAME] = {0};
    int vertex_count = 0;
    Edge *edges = malloc(MAX_EDGES * sizeof(Edge)); // Allocate on heap
    int edge_count = 0;

    // Parse input and build graph
    for (int i = 0; file_content[i] != NULL; i++)
    {
        char *line = strdup(file_content[i]);
        char *from = strtok(line, ":");

        int from_id = get_vertex_id(from, names, &vertex_count);
        char *to;

        while ((to = strtok(NULL, " \n")))
        {
            if (strlen(to) == 0)
            {
                continue;
            }

            int to_id = get_vertex_id(to, names, &vertex_count);
            edges[edge_count].from = from_id;
            edges[edge_count].to = to_id;
            edge_count++;
        }
        free(line);
    }

    srand(time(NULL));
    int best_cut = edge_count;
    int result = -1;

    // Karger's algorithm is probabilistic - it relies on random edge contractions.
    // We are doing 2000 attempts, but maybe we need to rerun.
    for (int attempt = 0; attempt < 2000 && result == -1; attempt++)
    {
        int *groups = malloc(MAX_NODES * sizeof(int));
        // Initialize each vertex in its own group
        for (int i = 0; i < vertex_count; i++)
        {
            groups[i] = i;
        }

        int curr_vertices = vertex_count;

        // Contract until only 2 groups remain
        while (curr_vertices > 2)
        {
            int e = rand() % edge_count;
            int g1 = groups[edges[e].from];
            int g2 = groups[edges[e].to];

            if (g1 == g2)
                continue;

            for (int i = 0; i < vertex_count; i++)
            {
                if (groups[i] == g2)
                {
                    groups[i] = g1;
                }
            }
            curr_vertices--;
        }

        int cut = 0;
        int g1 = groups[0];
        int size1 = 0, size2 = 0;

        for (int i = 0; i < vertex_count; i++)
        {
            if (groups[i] == g1)
            {
                size1++;
            }
            else
            {
                size2++;
            }
        }

        for (int i = 0; i < edge_count; i++)
        {
            if (groups[edges[i].from] != groups[edges[i].to])
            {
                cut++;
            }
        }

        if (cut < best_cut)
        {
            best_cut = cut;
            if (cut == 3)
            {
                result = size1 * size2;
            }
        }

        free(groups);
    }

    free(edges);
    return result;
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

    return 0;
}
