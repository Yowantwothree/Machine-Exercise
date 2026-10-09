#include <stdio.h>
#include <unistd.h>

int main(void) {
    printf("[CHILD]: PID %d, starts counting:\n", getpid());

    for (int i = 1; i <= 10; i++) {
        printf("[CHILD]: i = %d\n", i);
    }

    return 0;
}