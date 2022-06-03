#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    // For reading current directory
    struct dirent *de;

    // Open current directory
    DIR *dr = opendir(".");

    if (dr == NULL) {
        printf("Could not open current directory" );
        return 0;
    }

    // Print all files and directories
    while ((de = readdir(dr)) != NULL)
        if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0) {
            if (argc > 1 && strcmp(argv[1], "-l") == 0) {
                printf("%s\t", de->d_name);

                // Get the file
                struct stat fileStat;
                if(stat(de->d_name,&fileStat) < 0)    
                    return 1;

                // Print file information
                printf("File Size: %lld bytes\t",fileStat.st_size);
                printf("Blocks Allocated: %lld\t",fileStat.st_blocks);
                printf("Links: %d\t",fileStat.st_nlink);
            
                printf("File Permissions: ");
                printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
                printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
                printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
                printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
                printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
                printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
                printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
                printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
                printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
                printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
                printf("\t");

                printf("Inode: %llu\n",fileStat.st_ino);
                
            }
            else
                printf("%s\n", de->d_name);
        }

    closedir(dr);
    return 0;
}