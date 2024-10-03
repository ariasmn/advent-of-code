#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../helper.h"

#define NUM_REPLACEMENTS 10

// Define the replacements as a 2D array
char *replacements[NUM_REPLACEMENTS][2] = {
    {"zero", "z0o"},
    {"one", "o1e"},
    {"two", "t2o"},
    {"three", "t3e"},
    {"four", "f4r"},
    {"five", "f5e"},
    {"six", "s6x"},
    {"seven", "s7n"},
    {"eight", "e8t"},
    {"nine", "n9e"},
};

int part_1(char **file_content)
{
    int cal_sum = 0;
    for (size_t i = 0; file_content[i] != NULL; i++)
    {
        char *line = file_content[i];

        // We need to keep the first digit and the last digit.
        // Init both to -1 to make sure that we know how to continue.
        int first_digit = -1;
        int last_digit = -1;
        for (size_t j = 0; line[j] != '\0'; j++)
        {
            if (isdigit(line[j]))
            {
                // To convert the characters to digit, we need to understand how ASCII encoding works.
                // ASCII assigns an unique integer value to each char.
                // We need to know that the ASCII value of '0' is 48, '1' is 49... and so on.
                // Knowing this, the only thing we need to do, since all the digits are followed by each other,
                // is substract '0' to each digit that we find, to finally get the integer that we want.
                if (first_digit == -1)
                {
                    first_digit = line[j] - '0';
                }
                // Keep updating until we run out of lines.
                last_digit = line[j] - '0';
            }
        }
        // If only one digit, the digit repeats.
        if (last_digit == -1)
        {
            last_digit = first_digit;
        }
        // Construct the number and sum to total calibration.
        cal_sum += first_digit * 10 + last_digit;
    }

    return cal_sum;
}

int part_2(char **file_content)
{
    int cal_sum = 0;
    for (size_t i = 0; file_content[i] != NULL; i++)
    {
        // We modify the line to change the digit as words for digits.
        // As you can see, we are replacing the digit as word for the digit surrounded by the first and last character.
        // This is done because sometimes the digit as word first/last character may be used to define another digit as word.
        // Example: twone -> two1e -> t2o1e -> 21.
        char *line = file_content[i];
        char *modified_line = malloc(strlen(line) + 1);
        strcpy(modified_line, line);
        for (int i = 0; i < NUM_REPLACEMENTS; i++)
        {
            char *temp_line = str_replace(modified_line, replacements[i][0], replacements[i][1]);
            modified_line = realloc(modified_line, strlen(temp_line) + 1);
            strcpy(modified_line, temp_line);
        }

        // Once that we have the digits as ints, we need to use the same algorithm as part1.
        // Too lazy to create another func, so copy and paste.
        int first_digit = -1;
        int last_digit = -1;
        for (size_t j = 0; modified_line[j] != '\0'; j++)
        {
            if (isdigit(modified_line[j]))
            {
                if (first_digit == -1)
                {
                    first_digit = modified_line[j] - '0';
                }
                last_digit = modified_line[j] - '0';
            }
        }
        if (last_digit == -1)
        {
            last_digit = first_digit;
        }
        cal_sum += first_digit * 10 + last_digit;

        free(modified_line);
    }

    return cal_sum;
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

    free(file_content);

    return 0;
}
