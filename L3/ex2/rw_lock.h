/*************************************
* Lab 3 Exercise 1
* Name: Daryl Tan
* Student Id: A0132821R
* Lab Group: B17
*************************************
/*************************************
* Lab 3 Exercise 1
* Name: Leow Wen Bin
* Student Id: A0184415E
* Lab Group: B13
*************************************/

#ifndef __CS2106_RWLOCKV1_H_
#define __CS2106_RWLOCKV1_H_

#include <pthread.h> // To import the pthread_mutex_t type.

//TODO: You can modify the structure
typedef struct {
  pthread_mutex_t canWrite;
  pthread_mutex_t canAccess;
  int reader_count;
  int writer_count;
} rw_lock;

//Note: You are NOT allowed to change the function headers
void initialise(rw_lock* lock);
void writer_acquire(rw_lock* lock);
void writer_release(rw_lock* lock);
void reader_acquire(rw_lock* lock);
void reader_release(rw_lock* lock);
void cleanup(rw_lock* lock);

#endif