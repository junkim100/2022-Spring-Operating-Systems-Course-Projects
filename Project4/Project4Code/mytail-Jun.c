#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]) {
    unsigned long long pos;
    char str[100];

    // Open file
    FILE *fptr;
    fptr = fopen(argv[2], "r");

    // File not found error
    if (fptr == NULL) {
        printf("Error: File not found\n");
        exit(1);
    }
    
    // Go to End of file
    if (!fseek(fptr, 0, SEEK_END)) {
        pos = ftell(fptr);

        // Increase count when newline is found
        int count = 0;
        while (pos) {
            // Seek from end of line
            if (!fseek(fptr, --pos, SEEK_SET)) {
                if (fgetc(fptr) == '\n')
                    if (count++ == atoi(argv[1]))
                        break;
            }
        }

        // Print tail
        printf("These are the last %s lines of %s:", argv[1], argv[2]);
        while (fgets(str, sizeof(str), fptr))
            printf("%s", str);
    }
    printf("\n");

    // Close file
    fclose(fptr);
}