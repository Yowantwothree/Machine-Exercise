#include <stdio.h>
#include <unistd.h>

int main(void) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        // Child Process
        while (1) {
            printf("[CHILD]: PID %d, PPID %d\n", getpid(), getppid());
        }
    } else {
        // Parent Process
        while (1) {
            printf("[PARENT]: PID %d, PPID %d\n", getpid(), getppid());
        }
    }

    return 0;
}