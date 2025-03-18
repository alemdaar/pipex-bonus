#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();  // Create a child process

    if (pid == 0) {  // Child process
        printf("Hello from the child process! (PID: %d)\n", getpid());
        return (0);
    } else if (pid > 0) {
        printf("Hello from the parent process! (PID: %d)\n", getpid());
    } else {  // Error occurred in fork
        perror("fork failed");
        return 1;
    }
    pid = fork();  // Create a child process

    if (pid == 0) {  // Child process
        printf("Hello from the child process! (PID: %d)\n", getpid());
        return (1);
    } else if (pid > 0) {
        printf("Hello from the parent process! (PID: %d)\n", getpid());
    } else {  // Error occurred in fork
        perror("fork failed");
        return 1;
    }
    int *status1;
    wait(status1);
    printf ("wait is : %d\n", *status1);
    int *status2;
    wait(status2);
    printf ("wait is : %d\n", *status2);
    return 0;
}
