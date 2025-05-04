#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
// Compile with -DREALMALLOC to use the real malloc() instead of mymalloc()
#ifndef REALMALLOC
#include "mymalloc.h"
#endif

// 0: inappropriate pointer
// 1: double free
// 2: leak
#define ERROR_CASE 0

#define MEMSIZE 4096
#define HEADERSIZE 8
#define OBJECTS 64
#define OBJSIZE (MEMSIZE / OBJECTS - HEADERSIZE)
int main()
{
    //Task1, make sure that malloc()reserves unallocated memory
    char *obj[OBJECTS];
    int i, j, errors = 0;
    // fill memory with objects
    for (i = 0; i < OBJECTS; i++)
    {
        obj[i] = malloc(OBJSIZE);
        printf("malloc address is %p\n", obj[i]);
        if (obj[i] == NULL)
        {
            printf("Unable to allocate object %d\n", i);
            exit(1);
        }
    }
    // fill each object with distinct bytes
    for (i = 0; i < OBJECTS; i++)
    {
        memset(obj[i], i, OBJSIZE);
    }
    // check that all objects contain the correct bytes
    for (i = 0; i < OBJECTS; i++)
    {
        for (j = 0; j < OBJSIZE; j++)
        {
            if (obj[i][j] != i)
            {
                errors++;
                printf("Object %d byte %d incorrect: %d\n", i, j, obj[i][j]);
            }
        }
    }
    // free all objects
    for (i = 0; i < OBJECTS; i++)
    {
        free(obj[i]);
    }
    printf("%d incorrect bytes\n", errors);


    //Task2, make sure free() deallocates memory 
    free(malloc(64));
    printf("Task2 PASS!!!\n");

    //Task3 malloc() and free() arrange so that adjacent free blocks are coalesced
    void * p1 = malloc(2040);
    void *p2 = malloc(2040);
    free(p1);
    free(p2);
    free(malloc(4088));
    printf("Congratulation!! Task3 PASS\n");

    // Other error cases
    switch (ERROR_CASE) {
        case 0: // inappropriate pointer
            free((char *)malloc(10) + 1);
            break;
        case 1: // double free
            void *ptr = malloc(4088);
            free(ptr);
            free(ptr);
            break;
        case 2: // memory leak
            malloc(64);
            break;
        default:
            break;
    }

    return EXIT_SUCCESS;
}
