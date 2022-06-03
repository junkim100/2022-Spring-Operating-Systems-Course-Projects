#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
 
int main(int argc, char **argv)
{
    // End program if no arguments are given
    if(argc != 2)    
        return 1;
 
    // Get the file
    struct stat fileStat;
    if(stat(argv[1],&fileStat) < 0)    
        return 1;

    // Print file information
    printf("File Information of %s\n",argv[1]);
    printf("File Size: %lld bytes\n",fileStat.st_size);
    printf("Number of Blocks Allocated: %lld\n",fileStat.st_blocks);
    printf("Number of Links: %d\n",fileStat.st_nlink);
 
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
    printf("\n");

    printf("File Inode: %llu\n",fileStat.st_ino);
 
    return 0;
}