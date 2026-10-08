#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define BUFFER_SIZE 4096

int main(void)
{
    char source[256];
    char destination[256];
    char buffer[BUFFER_SIZE];

    printf("Enter source file: ");
    if (fgets(source, sizeof(source), stdin) == NULL) {
        fprintf(stderr, "Error reading source filename.\n");
        return EXIT_FAILURE;
    }

    printf("Enter destination file: ");
    if (fgets(destination, sizeof(destination), stdin) == NULL) {
        fprintf(stderr, "Error reading destination filename.\n");
        return EXIT_FAILURE;
    }

    // Remove newline characters
    source[strcspn(source, "\n")] = '\0';
    destination[strcspn(destination, "\n")] = '\0';

    // Open source file for reading
    int source_fd = open(source, O_RDONLY);

    if (source_fd == -1) {
        fprintf(stderr, "Error opening source file '%s': %s\n",
                source, strerror(errno));
        return EXIT_FAILURE;
    }

    // Open/create destination file
    int destination_fd = open(destination,
                               O_WRONLY | O_CREAT | O_TRUNC,
                               0644);

    if (destination_fd == -1) {
        fprintf(stderr, "Error opening destination file '%s': %s\n",
                destination, strerror(errno));
        close(source_fd);
        return EXIT_FAILURE;
    }

    ssize_t bytes_read;

    while ((bytes_read = read(source_fd, buffer, BUFFER_SIZE)) > 0) {

        ssize_t total_written = 0;

        while (total_written < bytes_read) {
            ssize_t bytes_written = write(destination_fd,
                                          buffer + total_written,
                                          bytes_read - total_written);

            if (bytes_written == -1) {
                fprintf(stderr, "Error writing to destination: %s\n",
                        strerror(errno));

                close(source_fd);
                close(destination_fd);
                return EXIT_FAILURE;
            }

            total_written += bytes_written;
        }
    }

    if (bytes_read == -1) {
        fprintf(stderr, "Error reading source file: %s\n",
                strerror(errno));

        close(source_fd);
        close(destination_fd);
        return EXIT_FAILURE;
    }

    if (close(source_fd) == -1) {
        perror("Error closing source file");
        close(destination_fd);
        return EXIT_FAILURE;
    }

    if (close(destination_fd) == -1) {
        perror("Error closing destination file");
        return EXIT_FAILURE;
    }

    printf("File copied successfully.\n");

    return EXIT_SUCCESS;
}