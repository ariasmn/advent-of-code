#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MORE_LINES 1024 // Define a constant for the initial number of lines to allocate.
#define MORE_CHARS 128  // Define a constant for the initial number of characters per line.

// Helper function to read the entire content of a file.
char **read_file(FILE *fptr)
{
    // Check if the file pointer is NULL, meaning the file couldn't be opened.
    if (fptr == NULL)
    {
        printf("File does not exist\n");
        return NULL;
    }

    // Allocate initial memory for an array of strings (lines).
    char **lines = malloc(sizeof(char *) * MORE_LINES);
    size_t total_lines = 0; // Counter for the total number of lines read.
    size_t total_chars = 0; // Counter for the number of characters in the current line.
    char c;                 // Variable to hold each character read from the file.

    // Read the file character by character until the end of the file is reached.
    while ((c = fgetc(fptr)) != EOF)
    {
        // If this is the first character in a line, allocate memory for the line.
        if (total_chars == 0)
        {
            lines[total_lines] = malloc(MORE_CHARS);
        }

        // Store the character in the current line.
        lines[total_lines][total_chars] = c;
        total_chars++; // Increment the character count for the current line.

        // Check if the current character is a newline character.
        if (c == '\n')
        {
            lines[total_lines][total_chars] = '\0'; // Null-terminate the current line.
            total_lines++;                          // Increment the line count.
            total_chars = 0;                        // Reset the character count for the next line.

            // If the number of lines exceeds the current allocated memory, reallocate more space.
            if (total_lines % MORE_LINES == 0)
            {
                lines = realloc(lines, sizeof(char *) * (total_lines + MORE_LINES));
            }
        }
        // If the line exceeds the current allocated memory for characters, reallocate more space.
        else if (total_chars % MORE_CHARS == 0)
        {
            lines[total_lines] = realloc(lines[total_lines], total_chars + MORE_CHARS);
        }
    }

    // Close the file since we've finished reading.
    fclose(fptr);

    // Null-terminate the array of lines so we know where the end of the array is.
    lines = realloc(lines, sizeof(char *) * (total_lines + 1));
    lines[total_lines] = NULL; // Set the last element to NULL.

    return lines; // Return the array of lines to the caller.
}

// Credits to someone on the internet, can't remember exactly from where I took it.
char *str_replace(char *orig, char *rep, char *with)
{
    char *result;  // the return string
    char *ins;     // the next insert point
    char *tmp;     // varies
    int len_rep;   // length of rep (the string to remove)
    int len_with;  // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count;     // number of replacements

    // sanity checks and initialization
    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for (count = 0; (tmp = strstr(ins, rep)); ++count)
    {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--)
    {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}

// string_equal gets two string and returns whether they are equal or not.
bool string_equal(char *str1, char *str2)
{
    int size = strlen(str1);
    if (size != (int)strlen(str2))
        return false;
    for (int i = 0; i < size; i++)
    {
        if (str1[i] != str2[i])
            return false;
    }
    return true;
}

// long_int_between gets three long long int and returns whether the value is between
// the other two long ints.
bool long_long_int_between(long long int value, long long int start, long long int end)
{
    if (value >= start && value <= end)
    {
        return true;
    }

    return false;
}
