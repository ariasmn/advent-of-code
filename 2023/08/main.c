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

long int gcd(long int a, long int b)
{
    // Find Minimum of a and b
    int result = ((a < b) ? a : b);
    while (result > 0) {
        // Check if both a and b are divisible by result
        if (a % result == 0 && b % result == 0) {
            break;
        }
        result--;
    }

    return result;
}

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

// Honestly, I tried bruteforcing this part to no aval (obviously).
// I checked Reddit for some information, and glad I did, since I honestly don't like when analyzing the input
// is somehow needed to correctly solve the problem.
// The idea is that all nodes between those ending with A linked to its node ending with Z are always linked between them, creating some sort of linked circular list. 
// This let us calculates how many steps are needed for each node __A to reach node __Z SEPARATELY
// and then just get the least common multiple (LCM) of all the steps needed on each node to get our result.
// We calculate the LCM by calculating GCD over all the steps counts.
long int part_2(char **file_content)
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

    long int steps_count[6]; // Bit of cheating here since we always have 6 nodes ending with A/ending with Z.
    int steps_count_arr_size = 0;

    for (int i = 0; i < nodes_arr_size; i++)
    {
        if (nodes[i].value[2] == 'A')
        {
            int steps = 0;
            int current_instruction = 0;
            char *next_node_value = nodes[i].value;

            while (next_node_value[2] != 'Z')
            {
                // Find the current node based on next_node_value.
                struct Node *current_node = NULL;
                for (int j = 0; j < nodes_arr_size; j++)
                {
                    if (strcmp(nodes[j].value, next_node_value) == 0)
                    {
                        current_node = &nodes[j];
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

                if (next_node_value[2] == 'Z')
                {
                    break;
                }

                current_instruction++;
                if (current_instruction == instructions_length - 1)
                {
                    current_instruction = 0;
                }
            }

            steps_count[steps_count_arr_size] = steps;
            steps_count_arr_size++;
        }
    }

    free(instructions);
    free(nodes);

    long int result = steps_count[0];
    for (int i = 1; i < steps_count_arr_size; i++) {
        result = result * steps_count[i] / gcd(result, steps_count[i]);
    }

    return result;
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
    printf("Part 2: %ld\n", part_2(file_content));
}