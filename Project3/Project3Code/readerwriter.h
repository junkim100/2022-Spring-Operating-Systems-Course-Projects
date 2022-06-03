// Referenced textbook page 351

#ifndef readerwriter_H
#define readerwriter_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct _rwlock_t {
    sem_t in;
    sem_t out;
    sem_t write;
    int inCheck;
    int outCheck;
    int isWaiting;
} rwlock_t;

void rwlock_init(rwlock_t *rw);

void rwlock_acquire_readlock(rwlock_t *rw);

void rwlock_release_readlock(rwlock_t *rw);

void rwlock_acquire_writelock(rwlock_t *rw);

void rwlock_release_writelock(rwlock_t *rw);

void reading_writing();

#endif