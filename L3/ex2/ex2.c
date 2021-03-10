/*************************************
* Lab 3 Exercise 2
* Name:
* Student Id: A????????
* Lab Group: B??
*************************************
Note: Duplicate the above and fill in
for the 2nd member if  you are on a team
*/

#include <pthread.h>
#include "rw_lock.h"
#include "stdlib.h"

void initialise(rw_lock* lock)
{
  //TODO: modify as needed
  lock->canAccess = *(pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
  lock->canWrite = *(pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
  pthread_mutex_init(&(lock->canAccess), NULL);
  pthread_mutex_init(&(lock->canWrite), NULL);
  lock->reader_count = 0;
  lock->writer_count = 0;
}

void writer_acquire(rw_lock* lock)
{
  //TODO: modify as needed
  pthread_mutex_lock(&(lock->canWrite));
  lock->writer_count++;
}

void writer_release(rw_lock* lock)
{
  lock->writer_count--;
  pthread_mutex_unlock(&(lock->canWrite));
}

void reader_acquire(rw_lock* lock)
{
  //TODO: modify as needed
  pthread_mutex_lock(&(lock->canAccess));
  if (lock->reader_count == 0) {
      pthread_mutex_lock(&(lock->canWrite));
  }
  lock->reader_count++;
  pthread_mutex_unlock(&(lock->canAccess));
}

void reader_release(rw_lock* lock)
{
  //TODO: modify as needed
    pthread_mutex_lock(&(lock->canAccess));
  lock->reader_count--;
  if (lock->reader_count == 0) {
      pthread_mutex_unlock(&(lock->canWrite));
  }
pthread_mutex_unlock(&(lock->canAccess));
}

void cleanup(rw_lock* lock)
{
  //TODO: modify as needed
  pthread_mutex_destroy(&(lock->canAccess));
  pthread_mutex_destroy(&(lock->canWrite));
}