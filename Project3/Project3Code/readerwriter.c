// Referenced textbook page 351

#include "readerwriter.h"

void rwlock_init(rwlock_t *rw) {
    sem_init(&rw->in, 0, 1);
    sem_init(&rw->out, 0, 1);
    sem_init(&rw->write, 0, 0);
    rw->inCheck = 0;
    rw->outCheck = 0;
    rw->isWaiting = 0;
}

void rwlock_acquire_readlock(rwlock_t *rw) {
    sem_wait(&rw->in);
    rw->inCheck++;
    sem_post(&rw->in);
}

void rwlock_release_readlock(rwlock_t *rw) {
    sem_wait(&rw->out);
    rw->outCheck++;
    if (rw->isWaiting==1 && rw->inCheck==rw->outCheck)
        sem_post(&rw->write);
    sem_post(&rw->out);
}

void rwlock_acquire_writelock(rwlock_t *rw) {
    sem_wait(&rw->in);
    sem_wait(&rw->out);
    if (rw->inCheck==rw->outCheck)
        sem_post(&rw->out);
    else {
        rw->isWaiting = 1;
        sem_post(&rw->out);
        sem_wait(&rw->write);
        rw->isWaiting = 0;
    }
}

void rwlock_release_writelock(rwlock_t *rw) {
    sem_post(&rw->in);
}

void reading_writing() {
    int x = 0;
    int T = rand()%32767;
    int i, j;
    for(i=0; i<T; i++)
        for(j=0; j<T; j++)
            x=i*j;
}