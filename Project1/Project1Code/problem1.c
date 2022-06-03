#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    pid_t pid = fork();
    if (pid<0) {
        perror("fork failed");
        exit(1);
    } else if (pid==0) {
        // Child
        char *args[argc-1];
        int i = 0;
        for (i = 0; i < argc-1; i++) {
            args[i] = argv[i+1];
        }
        args[argc-1] = NULL;

        execvp(args[0], args);
    }
    
    // Parent
    wait(NULL);
    printf("++++");

    return 0;
}