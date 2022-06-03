#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[])
{
    // Pipe
    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe failed");
        exit(1);
    }

    // Creating 2 Processes
    pid_t childA, childB;

    childA = fork();
    if (childA == 0) {
        // ChildA
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);

        execlp(argv[1], argv[1], NULL);
    }

    childB = fork();
    if (childB == 0) {
        // ChildB
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);

        execlp(argv[3], argv[3], NULL);
    }

    // Parent
    
    close(fd[0]);
    close(fd[1]);

    wait(NULL);
    printf("++++\n");

    return 0;
}