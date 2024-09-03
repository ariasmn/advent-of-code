#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../helper.h"

int main() {
    FILE *fptr = fopen("inputa.txt", "r");
    char **file_content = read_file(fptr);

    if (file_content == NULL) {
        return 1;
    }

    for (size_t i = 0; file_content[i] != NULL; i++) {
        printf("%s", file_content[i]);
        free(file_content[i]);  // Free each line
    }
    free(file_content);

    return 0;
}
