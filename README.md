## Thread_safe_malloc by De Lan

## Usage
```sh
$ make
$ cd thread_tests & make
```
#### Then these generated executable programs can verify the efficiency and accuracy of hread_safe_malloc



### Implementation of thread-safe malloc library

In this project, I implemented two different thread-safe versions (i.e. safe for concurrent access by different threads of a process) of the malloc() and free() functions. Both of the thread-safe malloc and free functions use the best fit allocation policy.


I implemented a doubly linked freeList to maintain the free memory. The freeList contains a list of blocks(having the information of freed memory that can be alloced for use) sorted by the pyisical address.


- In version 1 of the thread-safe malloc/free functions, I used lock-based synchronization to prevent race conditions that would lead to incorrect results. These functions are to be named:
```sh
     //Thread Safe malloc/free: locking version
     void *ts_malloc_lock(size_t size);
     void ts_free_lock(void *ptr);
```
I keep a global startFree as the head of the freeListLock and it is not declared as Thread Local Storage. Each thread shares the same freeList. And I add mutex when I need to adjust the freeList in each thread. And because of the locks I add, the time efficiency will be worse than no-lock version but the data segment size should be smaller.
![alt text](https://github.com/Alanlande/Socket_Hot_Potato_Game/blob/master/ringmaster.jpg "The main pending page")


- In version 2 of the thread-safe malloc/free functions, I did not use locks, with one exception. Because the sbrk() function is not thread-safe, I acquired a lock immediately before calling sbrk and released a lock immediately after calling sbrk(). These functions are to be named:
```sh
     //Thread Safe malloc/free: non-locking version 
     void *ts_malloc_nolock(size_t size);
     void ts_free_nolock(void *ptr);
```

I keep a global startFree as the head of the freeList for Thread Local Storage. When you call ts_malloc_nolock(size_t size), I traverse the freeList for best fit free block. If the suitable block is found, either split it into two blocks and replace current block with new block in the freeList, or delete it from the freeList. I not found, sbrk() new space and no need to update freeList. When you call ts_free_nolock(void \*ptr), I insert current block into freeList and check if the nextFree or prevFree blocks are physically adjacent. If so, merge them.


This design is thread safe because I keep only an independent freeList and its head in each thread. MALLOC in thread #1 will either change its own freeList or globally sbrk() and leave its freeList unchanged. If thread #1 FREEs its own block, that’s fine, just add new blocks to its sorted freeList. If thread #1 FREEs blocks MALLOCed by thread #2, just add the block to freeList in thread #1 and nothing in thread #2 will be touched. Then both MALLOC and FREE can happen concurrently in multiple threads.
![alt text](https://github.com/Alanlande/Socket_Hot_Potato_Game/blob/master/ringmaster.jpg "The main pending page")

### Test suite

In order to exercise and test the thread-safe malloc routines, pthread-based multi-threaded sample programs are provided. These programs will create threads which will make concurrent calls to the thread-safe malloc and free functions. Recall that concurrent execution means that multiple threads will call the malloc and free functions, and thus may be concurrently reading and updating any shared data structures used by the malloc routines (e.g. free list information).



These test cases (and a sample Makefile for the thread-safe library code) are provided in /thread_tests. The Makefile included in the test directory allows you to compile the tests to use either one of the two thread-safe versions (see the README.txt file for more details).





