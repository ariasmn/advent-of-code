#include <stdio.h>
#include <stdlib.h>

#define MORE_LINES 1024  // Define a constant for the initial number of lines to allocate.
#define MORE_CHARS 128   // Define a constant for the initial number of characters per line.

// Helper function to read the entire content of a file.
char **read_file(FILE *fptr) {
    // Check if the file pointer is NULL, meaning the file couldn't be opened.
    if (fptr == NULL) {
        printf("File does not exist\n");
        return NULL;
    }

    // Allocate initial memory for an array of strings (lines).
    char **lines = malloc(sizeof(char *) * MORE_LINES);
    size_t total_lines = 0;  // Counter for the total number of lines read.
    size_t total_chars = 0;  // Counter for the number of characters in the current line.
    char c;  // Variable to hold each character read from the file.

    // Read the file character by character until the end of the file is reached.
    while ((c = fgetc(fptr)) != EOF) {
        // If this is the first character in a line, allocate memory for the line.
        if (total_chars == 0) {
            lines[total_lines] = malloc(MORE_CHARS);
        }

        // Store the character in the current line.
        lines[total_lines][total_chars] = c;
        total_chars++;  // Increment the character count for the current line.

        // Check if the current character is a newline character.
        if (c == '\n') {
            lines[total_lines][total_chars] = '\0';  // Null-terminate the current line.
            total_lines++;  // Increment the line count.
            total_chars = 0;  // Reset the character count for the next line.

            // If the number of lines exceeds the current allocated memory, reallocate more space.
            if (total_lines % MORE_LINES == 0) {
                lines = realloc(lines, sizeof(char *) * (total_lines + MORE_LINES));
            }
        } 
        // If the line exceeds the current allocated memory for characters, reallocate more space.
        else if (total_chars % MORE_CHARS == 0) {
            lines[total_lines] = realloc(lines[total_lines], total_chars + MORE_CHARS);
        }
    }

    // Close the file since we've finished reading.
    fclose(fptr);

    // Null-terminate the array of lines so we know where the end of the array is.
    lines = realloc(lines, sizeof(char *) * (total_lines + 1));
    lines[total_lines] = NULL;  // Set the last element to NULL.

    return lines;  // Return the array of lines to the caller.
}
