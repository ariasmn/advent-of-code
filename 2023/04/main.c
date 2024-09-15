#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "../helper.h"

#define COUNT_WINNING_NUMBERS 10
#define COUNT_PLAYING_NUMBERS 25

struct Card
{
    int winning_numbers[COUNT_WINNING_NUMBERS];
    int playing_numbers[COUNT_PLAYING_NUMBERS];
};

int part_1(char **file_content)
{
    int card_arr_size = 0;
    struct Card *cards = NULL;

    for (int i = 0; file_content[i] != NULL; i++)
    {
        // We need to keep how many numbers we have inserted,
        // as well as if it's a winning number or a playing number.
        int number_count = 0;
        bool is_winning = true;

        // Remove extra whitespaces.
        char *modified_line = malloc(strlen(file_content[i]));
        strcpy(modified_line, file_content[i]);
        modified_line = str_replace(modified_line, "   ", " ");
        modified_line = str_replace(modified_line, "  ", " ");

        // Let's remove the "Card" placeholders.
        char card_buffer[15];
        snprintf(card_buffer, sizeof(card_buffer), "Card %d: ", i + 1);
        modified_line = str_replace(modified_line, card_buffer, "");

        int digit = 0;
        char number_string[3] = {'\0'}; // Max number is 99.

        cards = realloc(cards, (card_arr_size + 1) * sizeof(struct Card));

        for (int j = 0; modified_line[j] != '\0'; j++)
        {
            if (isdigit(modified_line[j]))
            {
                strncat(number_string, &modified_line[j], 1);
                continue;
            }

            if (number_string[0] != '\0')
            {
                // Terminate the string so "atoi" works.
                number_string[strlen(number_string)] = '\0';

                // Check flag to see what kind of number we are parsing.
                if (is_winning)
                {
                    cards[card_arr_size].winning_numbers[number_count] = atoi(number_string);
                }
                else
                {
                    cards[card_arr_size].playing_numbers[number_count] = atoi(number_string);
                }

                // Clear the number_string variable and increase the array size.
                memset(number_string, 0, sizeof(number_string));
                number_count++;
            }

            if (modified_line[j] == '|')
            {
                // We now start to read the playing numbers.
                // We also need to reset the number count.
                is_winning = false;
                number_count = 0;
            }
        }

        card_arr_size++;
    }

    // We have each card parsed, now let's calculate.
    // This could be cleaner, but whatever.
    int total_points = 0;
    for (int i = 0; i < card_arr_size; i++)
    {
        int card_points = 0;
        for (int j = 0; j < COUNT_PLAYING_NUMBERS; j++)
        {
            for (int k = 0; k < COUNT_WINNING_NUMBERS; k++)
            {
                if (cards[i].playing_numbers[j] == cards[i].winning_numbers[k])
                {
                    if (card_points == 0)
                    {
                        card_points = 1;
                    } else {
                        card_points *= 2;
                    }
                    break;
                }
            }
        }
        total_points += card_points;
    }

    return total_points;
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
}
