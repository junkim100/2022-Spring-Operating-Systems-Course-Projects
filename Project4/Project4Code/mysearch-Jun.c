#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>


void recursiveSearch(DIR *dr) {
    struct dirent *de;
    while ((de = readdir(dr)) != NULL) {
        if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0) {
            printf("%s\n", de->d_name);

            // New directory to pass into recursiveSearch function
            DIR *newdr = opendir(de->d_name);
            if (newdr != NULL)
                recursiveSearch(newdr);
        }
    }
}

int main(void)
{
    printf("Current Path %s\n", getcwd(NULL, 0));

    // For reading current directory
    struct dirent *de;

    // Open current directory
    DIR *dr = opendir(".");

    // Print all contents of current and subdirectories
    if (dr != NULL) {
        recursiveSearch(dr);
    }

    printf("Search has finished!\n");
}