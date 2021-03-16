/**
 * CS2106 AY 20/21 Semester 2 - Lab 3
 *
 * This file contains function definitions. Your implementation should go in
 * this file.
 */

#include "barrier.h"
#include "stdlib.h"

// Initialise barrier here
void barrier_init ( barrier_t *barrier, int count ) 
{
    //TODO: Implement the function
    barrier->count = count;
    sem_t *mutex = (sem_t*)malloc(sizeof(sem_t));
    sem_t *allReached = (sem_t*)malloc(sizeof(sem_t));
    sem_init(mutex, 0, 1);
    barrier->mutex = mutex;

    sem_init(allReached, 0, 0);
    barrier->allReached = allReached;
}

void barrier_wait ( barrier_t *barrier ) 
{
    //TODO: Implement the function
    sem_wait(barrier->mutex);
    barrier->count--;
    if (barrier-> count == 0) {
        sem_post(barrier->allReached);
    }
    sem_post(barrier->mutex);
    sem_wait(barrier->allReached);
    sem_post(barrier->allReached);
}

// Perform cleanup here if you need to
void barrier_destroy ( barrier_t *barrier ) 
{
    sem_destroy(barrier->mutex);
    sem_destroy(barrier->allReached);
}
