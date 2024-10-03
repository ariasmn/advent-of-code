#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "../helper.h"

struct Node
{
    char value[4];
    char left_value[4];
    char right_value[4];
};

int part_1(char **file_content)
{
    int nodes_arr_size = 0;
    struct Node *nodes = malloc(1 * sizeof(struct Node));

    int instructions_length = strlen(file_content[0]); 
    char *instructions = malloc(instructions_length + 1);
    strcpy(instructions, file_content[0]);

    // Start on the second line since the first line was manually parsed.
    for (int i = 1; file_content[i] != NULL; i++)
    {
        if (file_content[i][0] == '\n')
        {
            continue;
        }

        struct Node node;
        sscanf(file_content[i], "%3s = (%3s, %3s)", node.value, node.left_value, node.right_value);
        nodes = realloc(nodes, (nodes_arr_size + 1) * sizeof(struct Node));
        nodes[nodes_arr_size] = node;
        nodes_arr_size++;
    }

    int steps = 0;
    int current_instruction = 0;
    char *next_node_value = "AAA"; // Starting point.

    while (strcmp(next_node_value, "ZZZ") != 0)
    {
        // Find the current node based on next_node_value.
        struct Node *current_node = NULL;
        for (int i = 0; i < nodes_arr_size; i++)
        {
            if (strcmp(nodes[i].value, next_node_value) == 0)
            {
                current_node = &nodes[i];
                break;
            }
        }


        char instruction = instructions[current_instruction];
        if (instruction == 'L')
        {
            next_node_value = current_node->left_value;
        }
        else
        {
            next_node_value = current_node->right_value;
        }

        steps++;

        // Check if we reached "ZZZ" now after the move.
        if (strcmp(next_node_value, "ZZZ") == 0)
        {
            break;
        }

        current_instruction++;
        if (current_instruction == instructions_length - 1) // Substract one because we don't want to count the terminator.
        {
            current_instruction = 0;
        }
    }

    free(instructions);
    free(nodes);

    return steps;
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