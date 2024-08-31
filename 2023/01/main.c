#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../helper.h"

int main() {
    char *file_content = read_file("input.txt");

    if (file_content == NULL) {
        printf("Failed to read file.\n");
        return 1;
    }

    // Print the entire file content
    printf("%s\n", file_content);

    // Free the allocated memory
    free(file_content);

    return 0;
}
