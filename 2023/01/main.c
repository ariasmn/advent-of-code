#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../helper.h"

int part_1(char **file_content) {
    int cal_sum = 0;
    for (size_t i = 0; file_content[i] != NULL; i++) {
        char *line = file_content[i];

        // We need to keep the first digit and the last digit.
        // Init both to -1 to make sure that we know how to continue.
        int first_digit = -1;
        int last_digit = -1;
        for (size_t j = 0; line[j] != '\0'; j++) {
            if (isdigit(line[j])) {
                // To convert the characters to digit, we need to understand how ASCII encoding works.
                // ASCII assigns an unique integer value to each char.
                // We need to know that the ASCII value of '0' is 48, '1' is 49... and so on.
                // Knowing this, the only thing we need to do, since all the digits are followed by each other,
                // is substract '0' to each digit that we find, to finally get the integer that we want.
                if (first_digit == -1) {
                    first_digit = line[j] - '0';
                }
                // Keep updating until we run out of lines.
                last_digit = line[j] - '0';
            }
        }
        // If only one digit, the digit repeats.
        if (last_digit == -1) {
            last_digit = first_digit;
        }
        // Construct the number and sum to total calibration.
        cal_sum += first_digit * 10 + last_digit;
    }

    return cal_sum;
}

int main() {
    FILE *fptr = fopen("input.txt", "r");
    char **file_content = read_file(fptr);

    if (file_content == NULL) {
        return 1;
    }

    printf("Part 1: %d\n", part_1(file_content));

    return 0;
}

