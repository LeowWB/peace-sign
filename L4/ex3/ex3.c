/*************************************
* Lab 4 Exercise 3
* Name:
* Student Id: A????????
* Lab Group: B??
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mmalloc.h"

/**********************************************************
 * This is a "global" structure storing heap information
 * The "static" modifier restrict the structure to be
 *  visible only to functions in this file
 *********************************************************/
static heapMetaInfo hmi;


/**********************************************************
 * Quality of life helper functions / macros
 *********************************************************/
#define powOf2(E) (1 << E)

unsigned int log2Ceiling( unsigned int N )
/**********************************************************
 * Find the smallest S, such that 2^S >= N
 * S is returned
 *********************************************************/
{
    unsigned int s = 0, pOf2 = 1;

    while( pOf2 < N){
        pOf2 <<= 1;
        s++;
    }

    return s;
}

// not sure if allowed to import new libraries
int min(int a, int b) {
    if (a < b)
        return a;
    else
        return b;
}

unsigned int log2Floor( unsigned int N )
/**********************************************************
 * Find the largest S, such that 2^S <= N
 * S is returned
 *********************************************************/
{
    unsigned int s = 0, pOf2 = 1;

    while( pOf2 <= N){
        pOf2 <<= 1;
        s++;
    }

    return s-1;
}

unsigned int buddyOf( unsigned int addr, unsigned int lvl )
/**********************************************************
 * Return the buddy address of address (addr) at level (lvl)
 *********************************************************/
{
    unsigned int mask = 0xFFFFFFFF << lvl;
    unsigned int buddyBit = 0x0001 << lvl;

    return (addr & mask) ^ buddyBit;
}

partInfo* buildPartitionInfo(unsigned int offset)
/**********************************************************
 * Allocate a new partInfo structure and initialize the fields
 *********************************************************/
{
    partInfo *piPtr;

    piPtr = (partInfo*) malloc(sizeof(partInfo));

    piPtr->offset = offset;
    piPtr->nextPart = NULL;

    return piPtr;
}

void printPartitionList(partInfo* piPtr)
/**********************************************************
 * Print a partition linked list
 *********************************************************/
{
    partInfo* current;
    int count = 1;
    
    for ( current = piPtr; current != NULL; 
        current = current->nextPart){
        if (count % 8 == 0){
            printf("\t");
        }
        printf("[+%5d] ", current->offset);
        count++;
        if (count % 8 == 0){
            printf("\n");
        }
    }
    printf("\n");
}

void printHeapMetaInfo()
/**********************************************************
 * Print Heap Internal Bookkeeping Information
 *********************************************************/
{
    int i;

    printf("\nHeap Meta Info:\n");
    printf("===============\n");
    printf("Total Size = %d bytes\n", hmi.totalSize);
    printf("Start Address = %p\n", hmi.base);

    for (i = hmi.maxIdx; i >=0; i--){
        printf("A[%d]: ", i);
        printPartitionList(hmi.A[i] );
    }

}

void printHeap()
/**********************************************************
 * Print the content of the entire Heap 
 *********************************************************/
{
    //Included as last debugging mechanism.
    //Print the entire heap regions as integer values.

    int* array;
    int size, i;
    
    size = hmi.totalSize / sizeof(int);
    array = (int*)hmi.base;

    for ( i = 0; i < size; i++){
        if (i % 4 == 0){
            printf("[+%5d] |", i);
        }
        printf("%8d",array[i]);
        if ((i+1) % 4 == 0){
            printf("\n");
        }
    }
}

void printHeapStatistic()
/**********************************************************
 * Print Heap Usage Statistics
 *********************************************************/
{
    //TODO: Task 4. Calculate and report the various statistics

    printf("\nHeap Usage Statistics:\n");
    printf("======================\n");

   //Remember to preserve the message format!

    printf("Total Space: %d bytes\n", hmi.totalSize);


    int freeParts = 0;
    int freeSize = 0;
    int i = 0;

    for (i = 0; i <= hmi.maxIdx; i++) {
        partInfo *current = hmi.A[i];
        int countForThisLevel = 0;

        while ( current != NULL ){
            current = current->nextPart;
            countForThisLevel += 1;
        }

        freeParts += countForThisLevel;
        freeSize += countForThisLevel * powOf2(i);
    }

    printf("Total Free Partitions: %d\n", freeParts);
    printf("Total Free Size: %d bytes\n", freeSize);
    printf("Total Internal Fragmentation: %d bytes\n", hmi.internalFragTotal);
}

void addPartitionAtLevel( unsigned int lvl, unsigned int offset )
/**********************************************************
 * There is just a suggested approach. You are free NOT to use this.
 *    This function adds a new free partition with "offset" at hmi.A[lvl]
 *    If buddy is found, recursively (or repeatedly) perform merging and insert
 *      at higher level
 *********************************************************/
{
  
}

partInfo* removePartitionAtLevel(unsigned int lvl)
/**********************************************************
 * There is just a suggested approach. You are free NOT to use this.
 *    This function remove a free partition at hmi.A[lvl]
 *    Perform the "upstream" search if this lvl is empty AND perform
 *      the repeated split from higher level back to this level.
 * 
 * Return NULL if cannot find such partition (i.e. cannot sastify request)
 * Return the Partition Structure if found.
 *********************************************************/
{
    return NULL;
}

int setupHeap(int initialSize)
/**********************************************************
 * Setup a heap with "initialSize" bytes
 *********************************************************/
{
    void* base;

    base = sbrk(0);
    if(    sbrk(initialSize) == (void*)-1){
        printf("Cannot set break! Behavior undefined!\n");
        return 0;
    }

    hmi.base = base;

    hmi.totalSize = initialSize;
    hmi.internalFragTotal = 0;
    
    hmi.maxIdx = log2Floor(initialSize); // we can assume initial size is power of 2 (given)
    hmi.A = (partInfo**)malloc(sizeof(partInfo*) * (hmi.maxIdx+1));

    int i;

    for (i = 0; i < hmi.maxIdx; i++) {
        hmi.A[i] = NULL;
    }

    hmi.A[hmi.maxIdx] = buildPartitionInfo(0);

    //TODO: Task 1. Setup the rest of the bookkeeping info:
    //       hmi.A <= an array of partition linked list
    //       hmi.maxIdx <= the largest index for hmi.A[]
    //       

    return 1;
}


void* mymalloc(int size)
/**********************************************************
 * Mimic the normal "malloc()":
 *    Attempt to allocate a piece of free heap of (size) bytes
 *    Return the memory addres of this free memory if successful
 *    Return NULL otherwise 
 *********************************************************/
{
    //TODO: Task 2. Implement the allocation using buddy allocator
    int level = log2Ceiling(size);
    int splitCounts = 0;
    for (; level <= hmi.maxIdx; level++) {
        if (hmi.A[level] == NULL) {
            splitCounts++;
            continue;
        }
        break;
    }

    //level now contains the level from which we'll take a block.
    
    partInfo *blockToTake = hmi.A[level];
    hmi.A[level] = blockToTake->nextPart;
    int sizeOfBlock;

    while (splitCounts > 0) {
        splitCounts--;
        level--;
        sizeOfBlock = powOf2(level);
        partInfo *blockToPutBack = buildPartitionInfo(blockToTake->offset + sizeOfBlock);
        blockToPutBack->nextPart = blockToTake->nextPart;
        hmi.A[level] = blockToPutBack;
    }
    
    hmi.internalFragTotal += powOf2(log2Ceiling(size)) - size;
    return (void*)hmi.base + blockToTake->offset;
}

void myfree(void* address, int size)
/**********************************************************
 * Mimic the normal "free()":
 *    Attempt to free a previously allocated memory space
 *********************************************************/
{
    //TODO: Task 3. Implement the de allocation using buddy allocator
    int actualSize = size;
    int addr = address - hmi.base;
    int level = log2Ceiling(size);
    int buddyAddr = buddyOf(addr, level);
    size = powOf2(level);

    
    partInfo *current = hmi.A[level];
    partInfo *prev = NULL;
    while ( current != NULL ){
        if (current->offset == buddyAddr) {
            if (prev == NULL) {
                hmi.A[level] = current->nextPart;
            } else {
                prev->nextPart = current->nextPart;
            }
            return myfree(min(addr, buddyAddr) + hmi.base, size+actualSize);            
        } 
        prev = current;
        current = current->nextPart;
    }

    // buddy not found, just add the freed partition back.
    partInfo *newPart = buildPartitionInfo(addr);
    current = hmi.A[level];
    prev = NULL;
    while ( current != NULL ){
        if (current->offset > addr) {
            if (prev == NULL) {
                hmi.A[level] = newPart;
                newPart->nextPart = current;
            } else {
                prev->nextPart = newPart;
                newPart->nextPart = current;
            }
            hmi.internalFragTotal -= size - actualSize;
            return;
        } 
        prev = current;
        current = current->nextPart;
    }
    if (prev==NULL)
        hmi.A[level] = newPart;
    else
        prev->nextPart = newPart;
    hmi.internalFragTotal -= powOf2(log2Ceiling(actualSize)) - actualSize;
}
