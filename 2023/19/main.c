#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <search.h> // Didn't know that there was a "hash table" library in std...

#include "../helper.h"

#define MAX_WORKFLOWS 565

typedef struct
{
    char component;
    char operator;
    int value;
    char label[5];
} Rule;

typedef struct
{
    int num_rules;
    Rule rules[10];
} Workflow;

int part_1(char **file_content)
{
    // Initialize hash table for workflows
    hcreate(MAX_WORKFLOWS);

    Workflow workflows[MAX_WORKFLOWS] = {0};
    char label_arr[MAX_WORKFLOWS][5] = {0};
    int workflow_arr_size = 0;

    // First, parse workflows.
    for (int i = 0; file_content[i] != NULL; i++)
    {

        // Workflows are parsed now.
        if (file_content[i][0] == '\n')
        {
            break; // Transition to parts processing.
        }

        char label[5];
        char workflow_str[1024];

        // Extract workflow label.
        sscanf(file_content[i], "%[^{]{%[^}]}", label, workflow_str);
        strcpy(label_arr[workflow_arr_size], label);

        // Parse rules within the workflow.
        char *rule_token = strtok(workflow_str, ",");
        int rule_count = 0;

        while (rule_token != NULL)
        {
            Rule rule = {0};

            int comparison_match = sscanf(
                rule_token, "%c%c%d:%[^,}]",
                &rule.component,
                &rule.operator, & rule.value,
                rule.label);
            if (comparison_match != 4)
            {
                // If 4 items not matched, treat as default rule.
                sscanf(rule_token, "%[^,}]", rule.label);
                rule.operator= 0;
            }

            workflows[workflow_arr_size].rules[rule_count] = rule;
            workflows[workflow_arr_size].num_rules++;
            rule_count++;

            rule_token = strtok(NULL, ",");
        }

        // Add workflow to hash table.
        ENTRY e = {label_arr[workflow_arr_size], &workflows[workflow_arr_size]};
        hsearch(e, ENTER) == NULL;

        workflow_arr_size++;
    }

    // Process parts and calculate score.
    int total_score = 0;
    for (int i = workflow_arr_size + 1; file_content[i] != NULL; i++)
    {
        int x = 0, m = 0, a = 0, s = 0;
        sscanf(file_content[i], "{x=%d,m=%d,a=%d,s=%d}", &x, &m, &a, &s);

        int p[256] = {0};
        p['x'] = x;
        p['m'] = m;
        p['a'] = a;
        p['s'] = s;

        // Start with 'in' workflow.
        // This is per problem statement.
        Workflow *workflow = (Workflow *)hsearch((ENTRY){"in", NULL}, FIND)->data;

        while (workflow != NULL)
        {
            bool processed = false;
            for (int j = 0; j < workflow->num_rules; j++)
            {
                Rule rule = workflow->rules[j];
                bool valid = false;

                // Check rule conditions.
                if (rule.operator== 0)
                {
                    valid = true;
                }
                else if (rule.operator== '<' && p[rule.component]<rule.value)
                {
                    valid = true;
                }
                else if (rule.operator== '>' && p[rule.component]> rule.value)
                {
                    valid = true;
                }

                if (valid)
                {
                    if (strcmp(rule.label, "A") == 0)
                    {
                        total_score += p['x'] + p['m'] + p['a'] + p['s'];
                        workflow = NULL;
                        break;
                    }
                    else if (strcmp(rule.label, "R") == 0)
                    {
                        workflow = NULL;
                        break;
                    }

                    // Find next workflow.
                    ENTRY *e = hsearch((ENTRY){rule.label, NULL}, FIND);
                    if (e == NULL)
                    {
                        workflow = NULL;
                    }
                    else
                    {
                        workflow = (Workflow *)e->data;
                    }
                    processed = true;
                    break;
                }
            }

            if (!processed)
            {
                break;
            }
        }
    }

    // Cleanup hash table.
    hdestroy();

    return total_score;
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
