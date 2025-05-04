

#include <stdlib.h>
#include "mymalloc.h"
#include <stdbool.h>
#include <stdio.h>

void finish();
void initialize ();
#define MEMLENGTH 4096
static union{
    char bytes [MEMLENGTH];
    double not_used; // make sure the first address of element of the heap is 8 bytes 
}heap;
int initialized = 0;

typedef struct header{ //structure for header, should include size and status for free or not 
    short size; //for 2 bytes
    bool isFree; // for 1 byte
    short prevChunkSize; //meanning fo that?
}header;
void *mymalloc(size_t size, char *file, int line){
    // printf("the address of byte is %p\n",heap.bytes);
    // do we need to put these info into the heap?
    //if zero then turn it to 1.in order to only do once for the code in the if loop
    if(!initialized){
        initialized = 1;
        initialize();
        atexit(finish);
    }
    //TDL:find the first free chunk with enough size 
        //using pointer to traverse the whole heap 
    header *travPointer = (header *)heap.bytes;
    while(!travPointer -> isFree || travPointer -> size - 8 < (short)size){ //not enough size, or havent free yet
        //find the next header 
        travPointer = (header *)((char *)travPointer + travPointer -> size);

        if((char*)travPointer == heap.bytes+ MEMLENGTH){
            printf("malloc: Unable to allocate %zu bytes (%s:%d)\n",size, file, line); //s means the name of doc string, d means the line int
            return NULL;  
        }
    }
    //Align the size
    size = ((size + 7) / 8 )* 8;
    //TDL2:check if we need to split this chunk
    //s1: size == chunk size, do not split 
    //s2:remain size == 8, size + 8 == total size 
    if( (short)size >= travPointer -> size - 16){
        //situation1: do not split 
        travPointer->isFree = false;
            // printf("the address of tranvers pointer is %p and the difference is %ld\n",travPointer +8, (char*)travPointer+8 - (char*)heap.bytes);
        return (char *)travPointer + 8; 
    }

    //TDL3:Renew the size in prev chunk
    header *newPointer = (header *)((char *)travPointer + 8 + size);
    newPointer -> size = travPointer -> size - 8 - size; 
    newPointer -> isFree = true;
    newPointer -> prevChunkSize = travPointer -> size - newPointer -> size;
    travPointer -> size = newPointer ->prevChunkSize;
    travPointer -> isFree = false;
    header *nextPointer = (header *)((char *)newPointer + newPointer -> size);
    if((char*)nextPointer != heap.bytes+ MEMLENGTH){
        nextPointer->prevChunkSize = newPointer -> size;
    }
    return (char*)travPointer + 8;
}





void myfree(void *ptr, char *file, int line){ //ptr is the pointer that the ram is needed to be freed.
    //determine if the ptr is valued. if so, free the chunk; otherwise, return error
    //ptr is the address for the user data, payload address, not header address. 
    header *detePointer = (header *)heap.bytes; // create a pointer that point to the first header of the heap
    bool isFound = false; // a signal that show if the target chunk's address is founded
    while((char*)detePointer != heap.bytes+ MEMLENGTH){ // do it until reaching to the end of the array
        if(ptr != (char*) detePointer+ 8 ){ // +8 since the detePointer is pointed to the header's address, but ptr is the address of the user info.
            detePointer =(header *)((char *)detePointer + detePointer->size); //unit for size is 8 bytes, so we need to change detPointer's unit to char 
        }
        else{
            isFound  = true;
            break;
        }
    }
    if(isFound == false || detePointer->isFree == true ){ // s1: doesn't find the target. s2: double free situation
        printf("free: Inappropriate pointer (%s:%d)\n",file,line);
        exit(2);
    }
    //determine if the pre chunk is free. if so, merge it into one and repurpose it as the current chunk. 
    if(detePointer != (header *)heap.bytes){ //determine if the detePointer is the first header
        header *prevPointer = (header*)((char *)detePointer - detePointer->prevChunkSize);
        if(prevPointer->isFree){
            prevPointer->size = prevPointer->size + detePointer->size; //merge it into one
            detePointer = prevPointer;
        }        
    }
    header *nextPointer = (header *)((char *)detePointer + detePointer->size);
    if((char*)nextPointer!= heap.bytes+ MEMLENGTH && nextPointer->isFree){//determine if the next pointer the the end of the array
        detePointer->size = detePointer->size + nextPointer->size; //merge it into one
 
    }
    nextPointer = (header *)(detePointer->size + (char *)detePointer); //refresh the def of nextPointer 
    if( (char*)nextPointer!= heap.bytes+ MEMLENGTH){//determine if next Pointer is the end of the array
        nextPointer->prevChunkSize = detePointer->size; //refresh the next pointer's prevsize 
    }
    detePointer->isFree = true;
}

void initialize (){ //turn the heap into a large free chunk. we need to create a pointer to get address, then fill up the data in header(include size and status) 
     header *pointer = (header*) heap.bytes;  
     pointer -> size = MEMLENGTH; //another way: *((short *)heap.bytes) = MEMLENGTH. eg *p = 5 p is the pointer
     pointer -> isFree = true;  //another way *((bool *) (heap.bytes+2)) = false. 
}

void finish(){
    header *detePointer = (header *)heap.bytes; // create a pointer that point to the first header of the heap
    int numOfAllocChunk = 0;
    int totalSize = 0;
    while((char*)detePointer != heap.bytes+ MEMLENGTH){// do it until reaching to the end of the array
        if(!detePointer->isFree){// if a chunk is not free
            numOfAllocChunk ++;
            totalSize = totalSize + detePointer->size;
        }
        detePointer =(header *)((char *)detePointer + detePointer->size);
    }
    if(totalSize !=0){
        printf("mymalloc: %d bytes leaked in %d objects.\n",totalSize, numOfAllocChunk);
    }
}