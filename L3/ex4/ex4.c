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
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
*/

#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

#include "traffic_synchronizer.h"

//Using extern, you can use the global variables num_of_cars and num_of_segments from 
// ex4_driver.c in your code.
extern int num_of_cars;
extern int num_of_segments;

sem_t* segment_sems;
sem_t ensure_free_space_sem;

void initialise()
{
    segment_sems = malloc(sizeof (sem_t) * num_of_segments);
    for (int i = 0; i < num_of_segments; i++) {
    	sem_init(&segment_sems[i], 0, 1);
    }
    sem_init(&ensure_free_space_sem, 0, num_of_segments - 1);
}

void cleanup()
{
    for (int i = 0; i < num_of_segments; i++) {
	sem_destroy(&segment_sems[i]);
    }
    free(segment_sems);
    sem_destroy(&ensure_free_space_sem);
}

void* car(void* car)
{

    //This function modeles a thread 
    //A car: 
    //   1. should call enter_roundabout (...)
    //   2. followed by some calls to move_to_next_segment (...)
    //   3. finally call exit_roundabout (...)
    int entry_seg = ((car_struct*) car)->entry_seg;
    int exit_seg = ((car_struct*) car)->exit_seg;
	
    sem_wait(&ensure_free_space_sem);
    sem_wait(&segment_sems[entry_seg]);
    enter_roundabout(car);
    
    for (int i = NEXT(entry_seg, num_of_segments); i != NEXT(exit_seg, num_of_segments); i = NEXT(i, num_of_segments)) {
	    sem_wait(&segment_sems[i]);
	    move_to_next_segment(car);
	    sem_post(&segment_sems[PREV(i, num_of_segments)]);
    }

    exit_roundabout(car);
    sem_post(&segment_sems[exit_seg]); 
    sem_post(&ensure_free_space_sem);
    
    pthread_exit(NULL);
}
