#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "../helper.h"

typedef struct Lens
{
    char label[8];     // I think the largest one is shorter but whatever.
    int focal_length;  // Value.
    struct Lens *next; // For handling collision.
} Lens;

int get_hash(const char *str)
{
    int hash = 0;
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (!isprint(str[i]))
        {
            continue;
        }
        hash += str[i];
        hash *= 17;
        hash %= 256;
    }

    return hash;
}

int part_1(char **file_content)
{
    char *input = strdup(file_content[0]);
    char *step = strtok(input, ",");

    int total_val = 0;

    while (step != NULL)
    {
        total_val += get_hash(step);
        step = strtok(NULL, ",");
    }

    return total_val;
}

// Wording makes this way harder than it's in reality.
// The idea is to implement a hashmap, whose key will be the hashed label (everything that comes before '=' or '-').
// The hash is calculated using the same method in part 1.
// The hash calculated (box) is the key to the hashmap, which is the 'label' in the Lens struct.
// The "next" points to another Lens, because the same key can have multiple lenses.
int part_2(char **file_content)
{
    Lens *boxes[256] = {NULL};
    char *input = strdup(file_content[0]);
    char *step = strtok(input, ",");

    while (step != NULL)
    {
        char label[32] = {0};
        int i;

        // Get label (everything before = or -)
        for (i = 0; step[i] != '=' && step[i] != '-'; i++)
        {
            label[i] = step[i];
        }

        int box = get_hash(label);

        if (step[i] == '-')
        {
            // Remove lens
            Lens *curr = boxes[box];
            Lens *prev = NULL;

            // Same label, we need to move.
            while (curr && strcmp(curr->label, label) != 0)
            {
                prev = curr;
                curr = curr->next;
            }

            // If found, remove it
            if (curr)
            {
                if (prev)
                {
                    prev->next = curr->next;
                }
                else
                {
                    boxes[box] = curr->next;
                }
                free(curr);
            }
        }
        else
        {
            // Add/replace lens
            int focal = step[i + 1] - '0';
            Lens *curr = boxes[box];

            // Try to find existing lens
            while (curr && strcmp(curr->label, label) != 0)
            {
                curr = curr->next;
            }

            if (curr)
            {
                // Replace focal length
                curr->focal_length = focal;
            }
            else
            {
                // Add new lens at end
                Lens *new_lens = malloc(sizeof(Lens));
                strcpy(new_lens->label, label);
                new_lens->focal_length = focal;
                new_lens->next = NULL;

                if (!boxes[box])
                {
                    boxes[box] = new_lens;
                }
                else
                {
                    curr = boxes[box];
                    while (curr->next)
                        curr = curr->next;
                    curr->next = new_lens;
                }
            }
        }

        step = strtok(NULL, ",");
    }

    // Calculate result
    int result = 0;
    for (int i = 0; i < 256; i++)
    {
        int slot = 1;
        for (Lens *curr = boxes[i]; curr; curr = curr->next)
        {
            result += (i + 1) * slot * curr->focal_length;
            slot++;
        }
    }

    // Cleanup
    for (int i = 0; i < 256; i++)
    {
        while (boxes[i])
        {
            Lens *next = boxes[i]->next;
            free(boxes[i]);
            boxes[i] = next;
        }
    }

    free(input);
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
    printf("Part 2: %d\n", part_2(file_content));
}
