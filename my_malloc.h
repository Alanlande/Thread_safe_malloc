#ifndef __mymalloc_H__
#define __mymalloc_H__
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <pthread.h>
//definition of metablock, including the size, one doubly linked freelist
typedef struct mcb * mcBlock;
typedef struct mcb{
  size_t size;
  mcBlock prevFree;
  mcBlock nextFree;
}mcb;
//When the found free block'size is bigger than (size + blocksize), split it into two blocks
void splitBlock(mcBlock curr, size_t size);
//delete the found node from the free list
void deleteNode(mcBlock curr);
//merge two adjacent free block
void merge(mcBlock l, mcBlock r);
//add new block to freelist
void addBlock(mcBlock toFree, mcBlock curr);
//Thread Safe malloc/free: lock version 
void * ts_malloc_lock(size_t size);
void ts_free_lock(void *ptr);
//Thread Safe malloc/free: non-locking version 
void *ts_malloc_nolock(size_t size);
void ts_free_nolock(void *ptr);

#endif
