#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 4096

int main() {
    char source[1024];
    char destination[1024];
    char buffer[BUFFER_SIZE];

    printf("Enter source file: ");
    if (fgets(source, sizeof(source), stdin) == NULL) {
        fprintf(stderr, "Error reading source filename.\n");
        return 1;
    }

    printf("Enter destination file: ");
    if (fgets(destination, sizeof(destination), stdin) == NULL) {
        fprintf(stderr, "Error reading destination filename.\n");
        return 1;
    }

    source[strcspn(source, "\n")] = '\0';
    destination[strcspn(destination, "\n")] = '\0';

    // Open source file
    FILE *source_file = fopen(source, "rb");

    if (source_file == NULL) {
        fprintf(stderr, "Error opening source file '%s': %s\n",
                source, strerror(errno));
        return 1;
    }

    // Open/create destination file
    FILE *destination_file = fopen(destination, "wb");

    if (destination_file == NULL) {
        fprintf(stderr, "Error opening destination file '%s': %s\n",
                destination, strerror(errno));
        fclose(source_file);
        return 1;
    }

    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, source_file)) > 0) {

        size_t bytes_written =
            fwrite(buffer, 1, bytes_read, destination_file);

        if (bytes_written != bytes_read) {
            fprintf(stderr, "Error writing to destination file.\n");

            fclose(source_file);
            fclose(destination_file);
            return 1;
        }
    }

    if (ferror(source_file)) {
        fprintf(stderr, "Error reading source file.\n");

        fclose(source_file);
        fclose(destination_file);
        return 1;
    }

    if (fclose(source_file) != 0) {
        fprintf(stderr, "Error closing source file.\n");
        fclose(destination_file);
        return 1;
    }

    if (fclose(destination_file) != 0) {
        fprintf(stderr, "Error closing destination file.\n");
        return 1;
    }

    printf("File copied successfully.\n");

    return 0;
}