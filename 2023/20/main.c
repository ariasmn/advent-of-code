#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <search.h>

#include "../helper.h"

#define MAX_DESTINATIONS 10
#define MAX_LABEL_LENGTH 12
#define MAX_MODULES 5

typedef enum
{
    FLIP_FLOP,
    CONJUNCTION,
    BROADCAST,
} ModuleType;

typedef enum
{
    LOW,
    HIGH,
} Signal;

typedef struct
{
    char label[MAX_LABEL_LENGTH];
    ModuleType type;
    Signal signal;
    char destinations[MAX_DESTINATIONS][MAX_LABEL_LENGTH]; // This should be dynamically calculated to be honest...
    int destination_count;
    bool is_on;       // For flip flops. Off by default.
    Signal last_sent; // For conjunction.
} Module;

// Function to determine module type based on first character
ModuleType get_module_type(const char *label)
{
    switch (label[0])
    {
    case '%':
        return FLIP_FLOP;
    case '&':
        return CONJUNCTION;
    default:
        return BROADCAST;
    }
}

// Node structure for the queue
typedef struct QueueNode
{
    Module data;
    struct QueueNode *next;
} QueueNode;

typedef struct Queue
{
    QueueNode *front;
    QueueNode *rear;
} Queue;

// Create a new queue
Queue *create_queue()
{
    Queue *queue = malloc(sizeof(Queue));
    queue->front = queue->rear = NULL;
    return queue;
}

// Enqueue a copy of the module into the queue
void enqueue(Queue *queue, Module *module)
{
    QueueNode *new_node = malloc(sizeof(QueueNode));
    if (!new_node)
    {
        fprintf(stderr, "Error: Memory allocation failed for queue node.\n");
        exit(EXIT_FAILURE);
    }

    // Copy the module data
    memcpy(&new_node->data, module, sizeof(Module));
    new_node->next = NULL;

    if (queue->rear == NULL)
    {
        queue->front = queue->rear = new_node;
        return;
    }

    queue->rear->next = new_node;
    queue->rear = new_node;
}

// Dequeue an element from the queue (returns a copy of the Module)
Module dequeue(Queue *queue)
{
    if (queue->front == NULL)
    {
        fprintf(stderr, "Error: Attempting to dequeue from an empty queue.\n");
        exit(EXIT_FAILURE);
    }

    QueueNode *temp = queue->front;
    Module data;
    memcpy(&data, &temp->data, sizeof(Module)); // Copy the module data

    queue->front = queue->front->next;

    if (queue->front == NULL)
        queue->rear = NULL;

    free(temp);  // Free the node memory
    return data; // Return the copied data
}

// Check if the queue is empty
bool is_empty(Queue *queue)
{
    return queue->front == NULL;
}

// Destroy the queue and free its memory
void destroy_queue(Queue *queue)
{
    while (!is_empty(queue))
    {
        dequeue(queue);
    }
    free(queue);
}

long part_1(char **file_content)
{
    // Initialize hash table for modules.
    hcreate(MAX_MODULES);

    for (int i = 0; file_content[i] != NULL; i++)
    {
        char destinations_str[50] = {0};
        char raw_label[MAX_LABEL_LENGTH + 1] = {0};
        char *token;
        int dest_index = 0;

        // Clear whitespaces between the arrow.
        char *modified_line = malloc(strlen(file_content[i] + 1));
        strcpy(modified_line, file_content[i]);
        modified_line = str_replace(modified_line, " ", "");

        // Scan each line into its label and its destinations.
        sscanf(modified_line, "%11[^->]->%49[^\n]", raw_label, destinations_str);

        // Create the module.
        Module *module = malloc(sizeof(Module));
        module->type = get_module_type(raw_label);

        // Copy label, removing type prefix if necessary.
        if (module->type != BROADCAST)
        {
            strncpy(module->label, raw_label + 1, MAX_LABEL_LENGTH - 1);
        }
        else
        {
            strncpy(module->label, raw_label, MAX_LABEL_LENGTH - 1);
        }
        module->label[MAX_LABEL_LENGTH - 1] = '\0';
        module->signal = LOW;
        module->is_on = false;

        // Parse destinations.
        token = strtok(destinations_str, ",");
        while (token != NULL && dest_index < MAX_DESTINATIONS)
        {
            // Copy destination.
            strncpy(module->destinations[dest_index], token, MAX_LABEL_LENGTH - 1);
            module->destinations[dest_index][MAX_LABEL_LENGTH - 1] = '\0';
            dest_index++;

            // Get next destination.
            token = strtok(NULL, ",");
        }
        module->destination_count = dest_index;

        // Add the module to the hash table.
        ENTRY e = {.key = module->label, .data = module};
        hsearch(e, ENTER) == NULL;
    }

    // We now push the button 1000 times and do the calculations.
    long low_pulses = 0;
    long high_pulses = 0;
    // We need to get the broadcaster, which is the one that receives the button module signal.
    Module *broadcaster = (Module *)hsearch((ENTRY){"broadcaster", NULL}, FIND)->data;

    for (int j = 0; j < 1000; j++)
    {
        // Create queue and add the broadcaster.
        Queue *signal_queue = create_queue();
        broadcaster->signal = LOW;
        enqueue(signal_queue, broadcaster);

        while (!is_empty(signal_queue))
        {
            // Get the next element on the queue and do stuff.
            Module current = dequeue(signal_queue);

            // Count pulses.
            if (current.signal == LOW)
            {
                low_pulses++;
            }
            else
            {
                high_pulses++;
            }

            // Depending on the current module type, we need to do different things.
            switch (current.type)
            {
            case BROADCAST:
                // Get the destinations and add to queue.
                for (int c = 0; c < current.destination_count; c++)
                {
                    Module *dest = (Module *)hsearch((ENTRY){current.destinations[c], NULL}, FIND)->data;
                    dest->signal = current.signal;
                    enqueue(signal_queue, dest);
                }
                break;
            case FLIP_FLOP:
                // Only process LOW pulses
                if (current.signal == LOW)
                {
                    Module *save_last_sent = (Module *)hsearch((ENTRY){current.label, NULL}, FIND)->data;
                    // Toggle the state
                    save_last_sent->is_on = !save_last_sent->is_on;

                    // Send a pulse based on the new state
                    Signal new_signal = save_last_sent->is_on ? HIGH : LOW;
                    save_last_sent->last_sent = new_signal;

                    // Send the new signal to all destinations
                    for (int c = 0; c < current.destination_count; c++)
                    {
                        Module *dest = (Module *)hsearch((ENTRY){current.destinations[c], NULL}, FIND)->data;
                        dest->signal = new_signal;
                        enqueue(signal_queue, dest);
                    }
                }
                // Ignore HIGH pulses
                break;
            case CONJUNCTION:
                Signal to_send = LOW;
                for (int c = 0; c < current.destination_count; c++)
                {
                    Module *dest = (Module *)hsearch((ENTRY){current.destinations[c], NULL}, FIND)->data;
                    if (dest->last_sent == LOW)
                    {
                        to_send = HIGH;
                        break;
                    }
                }
                for (int c = 0; c < current.destination_count; c++)
                {
                    Module *dest = (Module *)hsearch((ENTRY){current.destinations[c], NULL}, FIND)->data;
                    dest->signal = to_send;
                    enqueue(signal_queue, dest);
                }
                break;
            }
        }
        destroy_queue(signal_queue);
    }

    return low_pulses * high_pulses;
}

int main()
{
    FILE *fptr = fopen("input.txt", "r");
    char **file_content = read_file(fptr);

    if (file_content == NULL)
    {
        return 1;
    }

    printf("Part 1: %ld\n", part_1(file_content));
    // printf("Part 2: %d\n", part_2(file_content));
}