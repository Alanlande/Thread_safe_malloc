#include "my_malloc.h"
__thread mcBlock startFree = NULL;  // start of freeList of no-lock version
//__thread mcBlock startFreeLock = NULL;  // start of freeList of the lock version
mcBlock startFreeLock = NULL;  // start of freeList of the lock version
size_t blockSize = sizeof(mcb);
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
//split the free block that is larger than (size + blocksize), link it in the free list
void splitBlock(mcBlock curr, size_t size){
  mcBlock newBlock = (mcBlock)((char*)curr + size + blockSize);
  newBlock->size = curr->size - size - blockSize;
  newBlock->prevFree = curr->prevFree;
  newBlock->nextFree = curr->nextFree;
  if(curr->nextFree)
    curr->nextFree->prevFree = newBlock;
  if(curr->prevFree)
    curr->prevFree->nextFree = newBlock;
  else{
    startFree = newBlock;
    startFreeLock = newBlock;
  }
  curr->size = size;
  curr->nextFree = NULL;
  curr->prevFree = NULL;
}
//delete the found apropriate free block from the free list
void deleteNode(mcBlock curr){
  if(curr->prevFree)
    curr->prevFree->nextFree = curr->nextFree;
  else{
    startFree = curr->nextFree;
    startFreeLock = curr->nextFree;
    if(startFree){
      startFree->prevFree = NULL;
      startFreeLock->prevFree = NULL;
    }
  }
  if(curr->nextFree)
    curr->nextFree->prevFree = curr->prevFree;
  curr->prevFree = NULL;
  curr->nextFree = NULL;
}
//merge two adjacent free blocks in the norm list
void merge(mcBlock l, mcBlock r){
  if((char *)l + l->size + blockSize == (char *)r){
    l->size += (blockSize + r->size);
    deleteNode(r);
  }
}

void addBlock(mcBlock toFree, mcBlock curr){
  if(!curr->prevFree){
    startFree = toFree;
    startFreeLock = toFree;
    toFree->nextFree = curr;
    curr->prevFree = toFree;
    merge(toFree, curr);
  }
  else{
    toFree->prevFree = curr->prevFree;
    toFree->nextFree = curr;
    curr->prevFree->nextFree = toFree;
    curr->prevFree = toFree;
    merge(toFree, curr);
    merge(toFree->prevFree, toFree);
  }
}
//Thread Safe malloc/free: lock version
void * ts_malloc_lock(size_t size){
  pthread_mutex_lock(&lock);
  mcBlock curr = startFreeLock;
  void * ans = NULL;
  mcBlock bestBlock = NULL;
  int foundBlock = 0;
  while(curr){
    if(curr->size == size){
      deleteNode(curr);
      ans = (void *)curr;
      break;
    }
    else if(curr->size > size){
      if(!foundBlock){
	bestBlock = curr;
	foundBlock = 1;
      }
      else{
	if(bestBlock->size > curr->size)
	  bestBlock = curr;
      }
    }
    curr = curr->nextFree;
  }
  if(!ans && foundBlock){
    ans = (void *)bestBlock;
    if(bestBlock->size <= size + blockSize)
      deleteNode(bestBlock);
    else
      splitBlock(bestBlock, size);
  }
  if(!ans){
    mcBlock new = sbrk(size + blockSize);
    new->size = size;
    new->prevFree = NULL;
    new->nextFree = NULL;
    ans = (void *)new;
  }
  pthread_mutex_unlock(&lock);
  return (char*)ans + blockSize;
}
void ts_free_lock(void *ptr){
  mcBlock toFree = (mcBlock)((char*)ptr - blockSize);
  pthread_mutex_lock(&lock);
  mcBlock curr = startFreeLock;
  mcBlock end = NULL;
  int inList = 0;
  if(!startFreeLock)
    startFreeLock = toFree;
  else{
    while(curr){
      if((char *)toFree < (char *)curr){
	inList = 1;
	addBlock(toFree, curr);
	break;
      }
      end = curr;
      curr = curr->nextFree;
    }
    if(!inList){
      if((char *)end + blockSize + end->size == (char *)toFree)
	end->size += (blockSize + toFree->size);
      else{
	end->nextFree = toFree;
	toFree->prevFree = end;
      }
    }
  }
  pthread_mutex_unlock(&lock);
}
//Thread Safe malloc/free: non-locking version
void *ts_malloc_nolock(size_t size){
  mcBlock curr = startFree;
  void * ans = NULL;
  mcBlock bestBlock = NULL;
  int foundBlock = 0;
  while(curr){
    if(curr->size == size){
      deleteNode(curr);
      ans = (void *)curr;
      break;
    }
    else if(curr->size > size){
      if(!foundBlock){
	bestBlock = curr;
	foundBlock = 1;
      }
      else{
	if(bestBlock->size > curr->size)
	  bestBlock = curr;
      }
    }
    curr = curr->nextFree;
  }
  if(!ans && foundBlock){
    ans = (void *)bestBlock;
    if(bestBlock->size <= size + blockSize)
      deleteNode(bestBlock);
    else
      splitBlock(bestBlock, size);
  }
  if(!ans){
    pthread_mutex_lock(&lock);
    mcBlock new = sbrk(size + blockSize);
    pthread_mutex_unlock(&lock);
    new->size = size;
    new->prevFree = NULL;
    new->nextFree = NULL;
    ans = (void *)new;
  }
  return (char*)ans + blockSize;
}
void ts_free_nolock(void *ptr){
  mcBlock toFree = (mcBlock)((char*)ptr - blockSize);
  mcBlock curr = startFree;
  mcBlock end = NULL;
  int inList = 0;
  if(!startFree)
    startFree = toFree;
  else{
    while(curr){
      if((char *)toFree < (char *)curr){
	inList = 1;
	addBlock(toFree, curr);
	break;
      }
      end = curr;
      curr = curr->nextFree;
    }
    if(!inList){
      if((char *)end + blockSize + end->size == (char *)toFree)
	end->size += (blockSize + toFree->size);
      else{
	end->nextFree = toFree;
	toFree->prevFree = end;
      }
    }
  }
}
