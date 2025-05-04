#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "mymalloc.h"
#define NUM_OPERATIONS 120
#define NUM_OPERATIONS 120

#define UPDATE_DURATION secs = end.tv_sec - start.tv_sec;\
    usecs = end.tv_usec - start.tv_usec;\
    duration = secs + usecs / 1000000.0;

int main() {
    char *pointers[NUM_OPERATIONS];
    int i;
    struct timeval start, end;
    long secs, usecs;
    double duration;


    // Task1: malloc() and immediately free() a 1-byte object, 120 times. testing performance for malloc and free 
    gettimeofday(&start, NULL);
    for (i = 0; i < NUM_OPERATIONS; i++) {
        char *ptr = (char *)malloc(1);
        free(ptr);
    }
    gettimeofday(&end, NULL);
    UPDATE_DURATION;
    printf("Task 1 Time: %f seconds\n", duration);

    // Task 2: Use malloc() to get 120 1-byte objects, storing the pointers in an array,
    // then use free() to deallocate the chunks
    gettimeofday(&start, NULL);
    for (i = 0; i < NUM_OPERATIONS; i++) {
        pointers[i] = (char *)malloc(1);
    }
    for (i = 0; i < NUM_OPERATIONS; i++) {
        free(pointers[i]);
    }
    gettimeofday(&end, NULL);
    UPDATE_DURATION;
    printf("Task 2 Time: %f seconds\n", duration);

    // Task 3: Randomly choose between allocating a 1-byte object and adding the pointer
    // to the array and deallocating a previously allocated object (if any), until you
    // have allocated 120 times. Deallocate any remaining objects.
    gettimeofday(&start, NULL);
    int alloc_count = 0;
    int index = 0;
    srand(time(NULL)); // initial seed, input the first number which is time

    while (alloc_count < NUM_OPERATIONS) {
        if (rand() % 2 == 0 || index == 0) {
            pointers[index++] = (char *)malloc(1);
            alloc_count++;
        } else {
            int randNum = rand()%index;
            free(pointers[randNum]);
            pointers[randNum] = pointers[index-1];
            index --;
        }
    }

    // Free any remaining allocated memory
    while (index > 0) {
        free(pointers[--index]);
    }
    gettimeofday(&end, NULL);
    UPDATE_DURATION;
    printf("Task 3 Time: %f seconds\n", duration);

     srand(time(NULL));

    
    //Self Designed 

    // Test 4: malloc small, medium, large size of chunk, and then free them 
    gettimeofday(&start, NULL);
    for (i = 0; i < NUM_OPERATIONS; i++) {
        char *ptr1 = (char *)malloc(1);
        char *ptr2 = (char *)malloc(10);
        char *ptr3 = (char *)malloc(100);
        free(ptr1);
        free(ptr2);
        free(ptr3);
    }
    gettimeofday(&end, NULL);
    UPDATE_DURATION;
    printf("Test 4 Time: %f seconds\n", duration);

    // Test 5: Mixed size allocation and deallocation with fragmentation simulation
    gettimeofday(&start, NULL);
    alloc_count = 0;
    index = 0;
    srand(time(NULL)); // initial seed, input the first number which is time

    while (alloc_count < 64) {
        if (rand() % 2 == 0 || index == 0) {
            pointers[index++] = (char *)malloc(rand()%2==0?24:56);
            alloc_count++;
        } else {
            int randNum = rand()%index;
            free(pointers[randNum]);
            pointers[randNum] = pointers[index-1];
            index --;
        }
    }

    // Free any remaining allocated memory
    while (index > 0) {
        free(pointers[--index]);
    }
    
    gettimeofday(&end, NULL);
    UPDATE_DURATION;
    printf("Test 5 Time: %f seconds\n", duration);

    return 0;
}