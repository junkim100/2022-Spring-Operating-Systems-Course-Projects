#include "readerwriter.h"

struct rwlock_t *lock;

void *readThread(rwlock_t *arg) {
    printf("Create reader\n");
    rwlock_acquire_readlock(arg);

    // Critical Section
    // Waste time
    printf("Reader's is in... reading\n");
    printf("Finished reading\n");
    reading_writing();

    rwlock_release_readlock(arg);

    return NULL;
}

void *writeThread(rwlock_t *arg) {
    rwlock_acquire_writelock(arg);
    printf("Create writer\n");

    // Critical Section
    // Waste time
    printf("Writer's is in... writing\n");
    printf("Finished writing\n");
    reading_writing();

    rwlock_release_writelock(arg);

    return NULL;
}

int main() {
    //variables
    char rw;
    FILE *file;
    int error;

    //open inputfile
    file = fopen("scenarios.txt", "r");

    //for threads
    pthread_t thread;

    lock = malloc(sizeof(rwlock_t));
    rwlock_init(&lock);

    if (file) {
        //scan input file
        printf("Scenario Starts:\n");
        while (fscanf(file, "%c", &rw) != EOF) {

            if (rw == 'r') {
                //if read create a thread to run the readThread
                error = pthread_create(&thread, NULL, (void *)readThread, &lock);
                if (error != 0)
                {
                    printf("Can't create thread.\n");
                    return 1;
                }
            }

            else if (rw == 'w') {
                //if write create a thread to run the writeThread
                error = pthread_create(&thread, NULL, (void *)writeThread, &lock);
                if (error != 0)
                {
                    printf("Can't create thread.\n");
                    return 1;
                }
            }
        }
    }

    //close file
    fclose(file);

    //exit threads
    pthread_exit(NULL);

    return 0;
}
