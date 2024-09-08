#ifndef HELPERS_H
#define HELPERS_H

// Function to read the entire content of a file.
// Returns a dynamically allocated array of strings containing the file content.
// Caller is responsible for freeing the allocated memory.
char **read_file(const FILE *fptr);

// Function to replace all the ocurrences in a string.
// Returns the modified string.
char *str_replace(char *orig, char *rep, char *with);

#endif // HELPERS_H
