#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid = fork();

    if (pid < 0) {
        return 1;
    }

    if (pid == 0) {
        // Child Process: Replace process image with child_task executable
        execlp("./me2_child", "./me2_child", NULL);
        
    } else {
        // Parent Process
        printf("[PARENT]: PID %d, waits for child with PID %d\n", getpid(), pid);
        
        int status;
        wait(&status);
        
        printf("[PARENT]: Child with PID %d finished and unloaded.\n", pid);
    }

    return 0;
}