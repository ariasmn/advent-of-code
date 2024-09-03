#ifndef HELPERS_H
#define HELPERS_H

// Function to read the entire content of a file
// Returns a dynamically allocated array of strings containing the file content
// Caller is responsible for freeing the allocated memory
char **read_file(const FILE *fptr);

#endif // HELPERS_H
