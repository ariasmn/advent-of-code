#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "../helper.h"

#define COUNT_WINNING_NUMBERS 10
#define COUNT_PLAYING_NUMBERS 25
#define MAX_NUMBER_CARDS 201

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
        free(modified_line);
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
                    // Could use pow here, but can't be bothered to link the libraries.
                    if (card_points == 0)
                    {
                        card_points = 1;
                    }
                    else
                    {
                        card_points *= 2;
                    }
                    break;
                }
            }
        }
        total_points += card_points;
    }

    free(cards);
    return total_points;
}

int part_2(char **file_content)
{
    int card_arr_size = 0;
    struct Card *cards = NULL;

    for (int i = 0; file_content[i] != NULL; i++)
    {
        int number_count = 0;
        bool is_winning = true;

        char *modified_line = malloc(strlen(file_content[i]));
        strcpy(modified_line, file_content[i]);
        modified_line = str_replace(modified_line, "   ", " ");
        modified_line = str_replace(modified_line, "  ", " ");

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
                number_string[strlen(number_string)] = '\0';

                if (is_winning)
                {
                    cards[card_arr_size].winning_numbers[number_count] = atoi(number_string);
                }
                else
                {
                    cards[card_arr_size].playing_numbers[number_count] = atoi(number_string);
                }

                memset(number_string, 0, sizeof(number_string));
                number_count++;
            }

            if (modified_line[j] == '|')
            {
                is_winning = false;
                number_count = 0;
            }
        }
        free(modified_line);
        card_arr_size++;
    }

    // This array holds the total number of cards per each card.
    int total_per_card[MAX_NUMBER_CARDS] = {0};
    // Init the array.
    for (int i = 0; i < card_arr_size; i++){
        total_per_card[i] = 1;
    }

    for (int i = 0; i < card_arr_size; i++)
    {
        int count_winning = 0;
        for (int j = 0; j < COUNT_PLAYING_NUMBERS; j++)
        {
            for (int k = 0; k < COUNT_WINNING_NUMBERS; k++)
            {
                if (cards[i].playing_numbers[j] == cards[i].winning_numbers[k])
                {
                    if (cards[i].playing_numbers[j] == 0)
                    {
                        continue;
                    }

                    count_winning++;
                    break;
                }
            }
        }

        // If no winning numbers, print and skip.
        if (count_winning == 0)
        {
            continue;
        }

        // Just uses how many winning numbers are there to 
        // update the number of cards.
        for (int l = 0; l < count_winning; l++)
        {
            total_per_card[i + 1 + l] += total_per_card[i];
        }
    }

    int total_cards = 0;
    for (int i = 0; i < MAX_NUMBER_CARDS; i++) {
        total_cards += total_per_card[i];
    }

    free(cards);
    return total_cards;
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
