#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>

#include "../helper.h"

#define MAX_GRID 141
#define INF INT_MAX

typedef struct
{
    int x, y;      // Position in the grid
    int direction; // Current direction (0: up, 1: right, 2: down, 3: left)
    int steps;     // Steps moved in the current direction
    int cost;      // Total heat loss
} State;

typedef struct Node
{
    State state;
    struct Node *next;
} Node;

typedef struct
{
    Node *head;
} PriorityQueue;

// Priority Queue Functions
void push(PriorityQueue *pq, State state)
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->state = state;
    new_node->next = NULL;
    if (pq->head == NULL || state.cost < pq->head->state.cost)
    {
        new_node->next = pq->head;
        pq->head = new_node;
        return;
    }
    Node *current = pq->head;
    while (current->next != NULL && current->next->state.cost <= state.cost)
    {
        current = current->next;
    }
    new_node->next = current->next;
    current->next = new_node;
}

State pop(PriorityQueue *pq)
{
    Node *temp = pq->head;
    State state = temp->state;
    pq->head = pq->head->next;
    free(temp);
    return state;
}

bool is_empty(PriorityQueue *pq)
{
    return pq->head == NULL;
}

// Directional Moves (up, right, down, left)
int dx[] = {-1, 0, 1, 0};
int dy[] = {0, 1, 0, -1};

// The idea is to just use a modified Dijkstra algorithm.
// To be honest, I just found the implementation of the algorithm somewhere online,
// and adapted it to my usage.
int part_1(char **file_content)
{
    int grid[MAX_GRID][MAX_GRID];

    // Parse input grid
    for (int i = 0; file_content[i] != NULL; i++)
    {
        for (int j = 0; file_content[i][j] != '\0'; j++)
        {
            grid[i][j] = file_content[i][j] - '0';
        }
    }

    // Initialize the minimum cost array
    int min_cost[MAX_GRID][MAX_GRID][4][4];
    for (int i = 0; i < MAX_GRID; i++)
    {
        for (int j = 0; j < MAX_GRID; j++)
        {
            for (int d = 0; d < 4; d++)
            {
                for (int s = 0; s <= 3; s++)
                {
                    min_cost[i][j][d][s] = INF;
                }
            }
        }
    }

    // Priority Queue setup
    PriorityQueue pq = {NULL};
    State start = {0, 0, -1, 0, 0}; // Start at (0, 0) with no direction and cost 0
    push(&pq, start);

    while (!is_empty(&pq))
    {
        State current = pop(&pq);

        // Check if destination is reached
        if (current.x == MAX_GRID - 1 && current.y == MAX_GRID - 1)
        {
            return current.cost; // Return total heat loss
        }

        // Explore moves: turn left, go straight, turn right
        for (int turn = -1; turn <= 1; turn++)
        {
            int new_direction = (current.direction + turn + 4) % 4;
            int new_steps = (turn == 0) ? current.steps + 1 : 1;

            if (new_steps > 3)
                continue; // Can't move more than 3 steps in the same direction

            int nx = current.x + dx[new_direction];
            int ny = current.y + dy[new_direction];

            if (nx < 0 || nx >= MAX_GRID || ny < 0 || ny >= MAX_GRID)
                continue; // Out of bounds

            // Add grid[nx][ny] only for subsequent moves, not the initial position
            int new_cost = current.cost + grid[nx][ny];
            // Special case: exclude initial position's cost.
            // This is a requirement in the problem statement.
            if (current.x == 0 && current.y == 0)
            {
                new_cost = grid[nx][ny];
            }

            if (new_cost < min_cost[nx][ny][new_direction][new_steps])
            {
                min_cost[nx][ny][new_direction][new_steps] = new_cost;
                State next_state = {nx, ny, new_direction, new_steps, new_cost};
                push(&pq, next_state);
            }
        }
    }

    return -1; // If no path is found
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
