#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper function to read the entire content of a file
char *read_file(const char *filename) {
    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL) {
        printf("File does not exist\n");
        exit(1);
    }

    // Move the file pointer to the end of the file to get the file size
    fseek(fptr, 0, SEEK_END);
    long file_size = ftell(fptr);
    rewind(fptr);  // Move the pointer back to the beginning of the file

    // Allocate memory to hold the file content +1 for the null terminator
    char *content = malloc(file_size + 1);
    if (content == NULL) {
        printf("Memory allocation failed\n");
        fclose(fptr);
        exit(1);
    }

    // Read the entire file into the content buffer
    fread(content, 1, file_size, fptr);
    content[file_size] = '\0';  // Null-terminate the string

    fclose(fptr);
    return content;
}
