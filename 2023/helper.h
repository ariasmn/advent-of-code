#ifndef HELPERS_H
#define HELPERS_H

// Macro to return the minimum between two values.
#define MIN(a, b) ((a) < (b) ? (a) : (b))

// Macro to return the maximum between two values.
#define MAX(a, b) ((a) > (b) ? (a) : (b))

// Function to read the entire content of a file.
// Returns a dynamically allocated array of strings containing the file content.
// Caller is responsible for freeing the allocated memory.
char **read_file(const FILE *fptr);

// Function to replace all the ocurrences in a string.
// Returns the modified string.
char *str_replace(char *orig, char *rep, char *with);

// Function to check whether two strings are equal or not.
// Returns a bool.
bool string_equal(char *str1, char *str2);

// Function to check whether a long long int is between two other long int.
// Returns a bool.
bool long_long_int_between(long long int value, long long int start, long long int end);

#endif // HELPERS_H
