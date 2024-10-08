#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "../helper.h"

// The max is 14, not 15, but we avoid to do the '-1' all the time with this.
#define POSSIBLE_CARDS 15

bool is_part_two = false;

enum HandType
{
    FIVE_OF_A_KIND,
    FOUR_OF_A_KIND,
    FULL_HOUSE,
    THREE_OF_A_KIND,
    TWO_PAIR,
    ONE_PAIR,
    HIGH_CARD,
};

struct Hand
{
    char *played_hand;
    int bid;
    enum HandType type;
    int rank;
};

int get_card_value(char card)
{
    switch (card)
    {
    case 'A':
        return 14;
    case 'K':
        return 13;
    case 'Q':
        return 12;
    case 'J':
        if (is_part_two)
        {
            return 1;
        }
        return 11;
    case 'T':
        return 10;
    }

    return atoi(&card);
}

// max_index returns the index of the card that has the highest frequency.
// If two frequencies are the same, it returns the one with the highest value.
// To be used only in part two.
int max_index(int *freq)
{
    int i, max_i = 0;
    float max = freq[0];
    // Start at 2 since we don't want to get the J.
    for (i = 2; i < POSSIBLE_CARDS; ++i)
    {
        if (freq[i] >= max)
        {
            max = freq[i];
            max_i = i;
        }
    }
    return max_i;
}

// get_hand_type returns a HandType based the frequency of the hand.
enum HandType get_hand_type(int *freq)
{
    int pairs = 0;
    int three_kind = 0;
    int four_kind = 0;
    int five_kind = 0;

    // Used for part two only.
    // We add the joker as freqs for the card with the highest freq. 
    if (is_part_two)
    {
        int joker_count = freq[1]; // 1 is the value of the joker.

        if (joker_count == 5) {
            return FIVE_OF_A_KIND;
        }

        int index_max_freq = max_index(freq);
        freq[index_max_freq] += joker_count;
    }

    // Get all the combinations
    for (int j = 0; j < POSSIBLE_CARDS; j++)
    {
        // this if statement is only for the part two.
        // the joker does not count towards pair and we count them separately to do logic later.
        if (is_part_two && j == 1)
        {
            continue;
        }

        if (freq[j] == 0)
        {
            continue;
        }

        switch (freq[j])
        {
        case 2:
            pairs++;
            break;
        case 3:
            three_kind++;
            break;
        case 4:
            four_kind++;
            break;
        case 5:
            five_kind++;
            break;
        }
    }

    if (five_kind == 1)
    {
        return FIVE_OF_A_KIND;
    }

    if (four_kind == 1)
    {
        return FOUR_OF_A_KIND;
    }

    if (three_kind == 1 && pairs == 1)
    {
        return FULL_HOUSE;
    }

    if (three_kind == 1)
    {
        return THREE_OF_A_KIND;
    }

    if (pairs == 2)
    {
        return TWO_PAIR;
    }

    if (pairs == 1)
    {
        return ONE_PAIR;
    }

    return HIGH_CARD;
}

// Comparison function for sorting hands by type and highest card
int compare_hands(const void *a, const void *b)
{
    struct Hand *hand1 = (struct Hand *)a;
    struct Hand *hand2 = (struct Hand *)b;

    // Compare hand types (lower number means a stronger hand)
    if (hand1->type != hand2->type)
    {
        return hand2->type - hand1->type;
    }

    // If the types are the same, compare by highest value card.
    // 5 is the number of cards of the hand.
    for (int i = 0; i < 5; i++)
    {
        if (get_card_value(hand1->played_hand[i]) == get_card_value(hand2->played_hand[i]))
        {
            continue;
        }

        return get_card_value(hand1->played_hand[i]) - get_card_value(hand2->played_hand[i]);
    }
}

int part_1(char **file_content)
{
    int hands_arr_size = 0;
    struct Hand *hands = malloc(1 * sizeof(struct Hand));

    for (int i = 0; file_content[i] != NULL; i++)
    {
        char *line = malloc(strlen(file_content[i]));
        strcpy(line, file_content[i]);

        char *played_hand = strtok(line, " ");
        char *played_bid = strtok(NULL, " ");

        // We store the frequency of apparition of each card in the hand.
        // This will be used to get the hand type.
        int freq[POSSIBLE_CARDS] = {0};
        struct Hand hand;
        hand.bid = atoi(played_bid);
        hand.played_hand = played_hand;

        for (int j = 0; played_hand[j] != '\0'; j++)
        {
            int card_value = get_card_value(played_hand[j]);
            freq[card_value]++;
        }
        hand.type = get_hand_type(freq);

        hands = realloc(hands, (hands_arr_size + 1) * sizeof(struct Hand));
        hands[hands_arr_size] = hand;
        hands_arr_size++;
    }

    // Sort hands by type and highest value card
    qsort(hands, hands_arr_size, sizeof(struct Hand), compare_hands);

    int total_winnings = 0;
    // Assign ranks to each hand (no two hands can have the same rank)
    // and calculate the result in the meantime.
    for (int j = 0; j < hands_arr_size; j++)
    {
        hands[j].rank = j + 1; // Ranks start from 1
        total_winnings += (hands[j].rank * hands[j].bid);
    }

    free(hands);

    return total_winnings;
}

int part_2(char **file_content)
{
    // Set this global variable to true, since it affects all the functions used.
    // Not the cleanest way of solving this, but whatever.
    is_part_two = true;

    int hands_arr_size = 0;
    struct Hand *hands = malloc(1 * sizeof(struct Hand));

    for (int i = 0; file_content[i] != NULL; i++)
    {
        char *line = malloc(strlen(file_content[i]));
        strcpy(line, file_content[i]);

        char *played_hand = strtok(line, " ");
        char *played_bid = strtok(NULL, " ");

        // We store the frequency of apparition of each card in the hand.
        // This will be used to get the hand type.
        int freq[POSSIBLE_CARDS] = {0};
        struct Hand hand;
        hand.bid = atoi(played_bid);
        hand.played_hand = played_hand;

        for (int j = 0; played_hand[j] != '\0'; j++)
        {
            int card_value = get_card_value(played_hand[j]);
            freq[card_value]++;
        }
        hand.type = get_hand_type(freq);


        hands = realloc(hands, (hands_arr_size + 1) * sizeof(struct Hand));
        hands[hands_arr_size] = hand;
        hands_arr_size++;
    }

    // Sort hands by type and highest value card
    qsort(hands, hands_arr_size, sizeof(struct Hand), compare_hands);

    int total_winnings = 0;
    // Assign ranks to each hand (no two hands can have the same rank)
    // and calculate the result in the meantime.
    for (int j = 0; j < hands_arr_size; j++)
    {
        hands[j].rank = j + 1; // Ranks start from 1
        total_winnings += (hands[j].rank * hands[j].bid);
    }

    free(hands);

    return total_winnings;
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
