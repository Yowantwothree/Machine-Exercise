#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 256

int main(void) {
    int pipe1[2]; // Pipe 1: Parent -> Child
    int pipe2[2]; // Pipe 2: Child -> Parent

    /* Create both ordinary pipes */
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("Pipe creation failed");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }

    if (pid > 0) {
        /* ---------------- PARENT PROCESS ---------------- */
        close(pipe1[0]);
        close(pipe2[1]);

        char input_msg[BUFFER_SIZE];
        char read_buffer[BUFFER_SIZE];

        /* Prompt user for message input */
        printf("Input string message: ");
        if (fgets(input_msg, sizeof(input_msg), stdin) != NULL) {
            /* Remove trailing newline character from fgets */
            input_msg[strcspn(input_msg, "\n")] = '\0';
        }

        printf("PARENT(%d): Sending [%s] to Child\n", getpid(), input_msg);
        fflush(stdout);

        /* Send original message to Child via Pipe 1 */
        write(pipe1[1], input_msg, strlen(input_msg) + 1);
        close(pipe1[1]); /* Done writing to Pipe 1 */

        /* Read modified message back from Child via Pipe 2 */
        read(pipe2[0], read_buffer, sizeof(read_buffer));
        close(pipe2[0]); /* Done reading from Pipe 2 */

        printf("PARENT(%d): Received [%s] from Child\n", getpid(), read_buffer);
        fflush(stdout);

        /* Wait for child process to clean up */
        wait(NULL);

    } else {
        /* ---------------- CHILD PROCESS ---------------- */
        close(pipe1[1]);
        close(pipe2[0]);

        char received_msg[BUFFER_SIZE];

        /* Read original message from Parent via Pipe 1 */
        read(pipe1[0], received_msg, sizeof(received_msg));
        close(pipe1[0]); /* Done reading from Pipe 1 */

        printf("CHILD(%d): Received message\n", getpid());
        printf("CHILD(%d): Reversing the case of the string and sending to Parent\n", getpid());
        fflush(stdout);

        /* Reverse case of each character */
        for (int i = 0; received_msg[i] != '\0'; i++) {
            if (islower((unsigned char)received_msg[i])) {
                received_msg[i] = toupper((unsigned char)received_msg[i]);
            } else if (isupper((unsigned char)received_msg[i])) {
                received_msg[i] = tolower((unsigned char)received_msg[i]);
            }
        }

        /* Send modified message back to Parent via Pipe 2 */
        write(pipe2[1], received_msg, strlen(received_msg) + 1);
        close(pipe2[1]); /* Done writing to Pipe 2 */

        exit(0);
    }

    return 0;
}