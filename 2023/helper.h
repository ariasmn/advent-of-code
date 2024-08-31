#ifndef HELPERS_H
#define HELPERS_H

// Function to read the entire content of a file
// Returns a dynamically allocated string containing the file content
// Caller is responsible for freeing the allocated memory
char *read_file(const char *filename);

#endif // HELPERS_H
