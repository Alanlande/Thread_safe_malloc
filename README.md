## Thread_safe_malloc by De Lan

## Usage
```sh
$ make
$ cd thread_tests & make
```
#### Then these generated executable programs can verify the efficiency and accuracy of hread_safe_malloc



### Implementation of thread-safe malloc library

In this project, I implemented two different thread-safe versions (i.e. safe for concurrent access by different threads of a process) of the malloc() and free() functions. Both of the thread-safe malloc and free functions use the best fit allocation policy.

- In version 1 of the thread-safe malloc/free functions, I used lock-based synchronization to prevent race conditions that would lead to incorrect results. These functions are to be named:
```sh
     //Thread Safe malloc/free: locking version
     void *ts_malloc_lock(size_t size);
     void ts_free_lock(void *ptr);
```

- In version 2 of the thread-safe malloc/free functions, I did not use locks, with one exception. Because the sbrk function is not thread-safe, I acquired a lock immediately before calling sbrk and release a lock immediately after calling sbrk. These functions are to be named:
```sh
     //Thread Safe malloc/free: non-locking version 
     void *ts_malloc_nolock(size_t size);
     void ts_free_nolock(void *ptr);
```
To provide necessary synchronization for the locking version, I used support from the pthread library and used synchronization primitives (e.g. pthread_mutex_t, etc.). Also, Thread-Local Storage is useful in this project.



### Test suite

In order to exercise and test the thread-safe malloc routines, pthread-based multi-threaded sample programs are provided. These programs will create threads which will make concurrent calls to the thread-safe malloc and free functions. Recall that concurrent execution means that multiple threads will call the malloc and free functions, and thus may be concurrently reading and updating any shared data structures used by the malloc routines (e.g. free list information).



These test cases (and a sample Makefile for the thread-safe library code) are provided in /thread_tests. The Makefile included in the test directory allows you to compile the tests to use either one of the two thread-safe versions (see the README.txt file for more details).





