// In this problem, for both part 1 and 2 I heavily relied on a solution I found on Reddit to be honest.
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
    char name[5];
    int num_rules;
    Rule rules[10];
} Workflow;


typedef struct {
    int start;
    int end;
} Range;

int part_1(char **file_content)
{
    // Initialize hash table for workflows.
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

// Recursive function to calculate the number of posibilities of a given workflow.
// The starting range will be, as said in the statement, 1 to 4000.
long long count_possibilities(Range ranges[4], char* label, Workflow* workflows, char label_arr[MAX_WORKFLOWS][5], int workflow_arr_size) {
    // Direct accept/reject cases.
    if (strcmp(label, "A") == 0) {
        return (long long)(ranges[0].end - ranges[0].start + 1) *
               (ranges[1].end - ranges[1].start + 1) *
               (ranges[2].end - ranges[2].start + 1) *
               (ranges[3].end - ranges[3].start + 1);
    }
    if (strcmp(label, "R") == 0) {
        return 0;
    }

    Workflow* workflow = (Workflow*)hsearch((ENTRY){label, NULL}, FIND)->data;
    long long sum = 0;

    for (int i = 0; i < workflow->num_rules; i++) {
        Rule rule = workflow->rules[i];
        
        // Default rule (no operator).
        if (rule.operator == 0) {
            sum += count_possibilities(ranges, rule.label, workflows, label_arr, workflow_arr_size);
            break;
        }
        
        // Component lookup for x,m,a,s.
        int component_index;
        switch (rule.component) {
            case 'x': component_index = 0; break;
            case 'm': component_index = 1; break;
            case 'a': component_index = 2; break;
            case 's': component_index = 3; break;
            default: continue;
        }

        if (rule.operator == '<' && rule.value > ranges[component_index].start) {
            if (rule.value <= ranges[component_index].end) {
                // Create a new range full of values less than rule.value.
                Range new_ranges[4];
                memcpy(new_ranges, ranges, sizeof(Range) * 4);
                new_ranges[component_index].end = rule.value - 1;
                
                // Set current range to values greater than or equal to rule.value.
                ranges[component_index].start = rule.value;

                // Count possibilities at the new label with the new range.
                sum += count_possibilities(new_ranges, rule.label, workflows, label_arr, workflow_arr_size);
            }
            // Entire range follows the rule, count possibilities at new label.
            else {
                return sum + count_possibilities(ranges, rule.label, workflows, label_arr, workflow_arr_size);
            }
        }
        else if (rule.operator == '>' && rule.value < ranges[component_index].end) {
            if (rule.value >= ranges[component_index].start) {
                // Create a new range full of values greater than rule.value.
                Range new_ranges[4];
                memcpy(new_ranges, ranges, sizeof(Range) * 4);
                new_ranges[component_index].start = rule.value + 1;
                
                // Set current range to values less than or equal to rule.value.
                ranges[component_index].end = rule.value;

                // Count possibilities at the new label with the new range.
                sum += count_possibilities(new_ranges, rule.label, workflows, label_arr, workflow_arr_size);
            }
            // Entire range follows the rule, count possibilities at new label.
            else {
                return sum + count_possibilities(ranges, rule.label, workflows, label_arr, workflow_arr_size);
            }
        }
    }
    return sum;
}

// Copy pasting, again, the parsing from part 1.
// Just the workflow since the part ratings are not needed.
long long part_2(char **file_content) {
    hcreate(MAX_WORKFLOWS);
    Workflow workflows[MAX_WORKFLOWS] = {0};
    char label_arr[MAX_WORKFLOWS][5] = {0};
    int workflow_arr_size = 0;

    for (int i = 0; file_content[i] != NULL; i++) {
        if (file_content[i][0] == '\n') {
            break;
        }

        char label[5];
        char workflow_str[1024];
        
        sscanf(file_content[i], "%[^{]{%[^}]}", label, workflow_str);
        strcpy(label_arr[workflow_arr_size], label);
        
        char *rule_token = strtok(workflow_str, ",");
        int rule_count = 0;
        
        while (rule_token != NULL) {
            Rule rule = {0};
            int comparison_match = sscanf(
                rule_token, "%c%c%d:%[^,}]",
                &rule.component,
                &rule.operator, &rule.value,
                rule.label
            );
            
            if (comparison_match != 4) {
                // If 4 items not matched, treat as default rule.
                sscanf(rule_token, "%[^,}]", rule.label);
                rule.operator = 0;
            }
            
            workflows[workflow_arr_size].rules[rule_count] = rule;
            workflows[workflow_arr_size].num_rules++;
            rule_count++;
            
            rule_token = strtok(NULL, ",");
        }
        
        // Add workflow to hash table.
        ENTRY e = {label_arr[workflow_arr_size], &workflows[workflow_arr_size]};
        hsearch(e, ENTER);
        
        workflow_arr_size++;
    }

    // Initialize ranges.
    Range ranges[4];
    for (int i = 0; i < 4; i++) {
        ranges[i] = (Range){1, 4000};
    }

    // Calculate possibilities.
    long long result = count_possibilities(ranges, "in", workflows, label_arr, workflow_arr_size);

    // Cleanup hash table.
    hdestroy();

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
    printf("Part 2: %lld\n", part_2(file_content));
}
