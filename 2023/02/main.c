#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "../helper.h"

#define MAX_BLUE 14
#define MAX_RED 12
#define MAX_GREEN 13
#define NUM_REPLACEMENTS 3

// Define the replacements as 2D array.
// With spaces, to remove them too.
char *replacements[NUM_REPLACEMENTS][2] = {
    {" blue", "b"},
    {" red", "r"},
    {" green", "g"},
};

int part_1(char **file_content)
{
    int sum_game_id = 0;

    // My idea for this is to clean up each line before processing.
    // We don't need the Game ID, since we already have the iteration.
    // We also want to replace the colors for a single char, to easily,
    // detect which color we are processing and sum to the total.
    // Similar approach to day 1 actually.
    for (int i = 0; file_content[i] != NULL; i++)
    {
        char *modified_line = malloc(strlen(file_content[i]) + 1);
        strcpy(modified_line, file_content[i]);

        // No need to reallocate since the string would be smaller.
        // First, remove the game id.
        char game_buffer[12];
        snprintf(game_buffer, sizeof(game_buffer), "Game %d: ", i + 1);
        modified_line = str_replace(modified_line, game_buffer, "");

        // Remove spaces and commas.
        modified_line = str_replace(modified_line, ", ", "");
        modified_line = str_replace(modified_line, "; ", ";");

        // Add semicolon to end of line.
        // Seems like a bit too much, but don't know if there's a better way to do this.
        modified_line = str_replace(modified_line, "\n", "");
        size_t mod_len = strlen(modified_line);
        modified_line = realloc(modified_line, mod_len + 2);
        modified_line[mod_len] = ';';
        modified_line[mod_len + 1] = '\0';

        // Replace the colors for letters, to easily identify.
        for (int i = 0; i < NUM_REPLACEMENTS; i++)
        {
            modified_line = str_replace(modified_line, replacements[i][0], replacements[i][1]);
        }

        // We need to keep the last number, in case it's a number > 9
        int current_number = 0;
        int game_blue = 0;
        int game_red = 0;
        int game_green = 0;
        bool playable = true;

        // Now, iterate the clean string to calculate the number of cubes of each color.
        for (int j = 0; modified_line[j] != '\0'; j++)
        {
            if (!playable)
            {
                break;
            }

            // If it's not a digit or end round, skip it, because is the color identification.
            // And we've already read it.
            if (!isdigit(modified_line[j]) && modified_line[j] != ';')
            {
                continue;
            }

            // If it's the end round, calculate if possible and change flag if needed.
            if (modified_line[j] == ';')
            {
                if (game_blue > MAX_BLUE || game_red > MAX_RED || game_green > MAX_GREEN)
                {
                    playable = false;
                }

                current_number = 0;
                game_blue = 0;
                game_red = 0;
                game_green = 0;

                continue;
            }

            // If the current number is not 0, it means that it's the second digit, so we need to sum.
            // Else, is just the current number.
            if (current_number != 0) {
                current_number += (modified_line[j] - '0');
            } else {
                current_number = (modified_line[j] - '0');
            }

            // If there it's a with 2 digits, calculate and skip for the next digit.
            if (isdigit(modified_line[j+1]))
            {
                current_number *= 10;
                continue;
            }

            // Read the color and sum to the total game
            if (modified_line[j + 1] == 'b')
            {
                game_blue += current_number;
                current_number = 0;
            }
            else if (modified_line[j + 1] == 'r')
            {
                game_red += current_number;
                current_number = 0;
            }
            else if (modified_line[j + 1] == 'g')
            {
                game_green += current_number;
                current_number = 0;
            }
        }
        free(modified_line);

        // Don't sum game id if the game is impossible to play.
        if (playable)
        {
            sum_game_id += i + 1;
        }
    }

    return sum_game_id;
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

    free(file_content);

    return 0;
}