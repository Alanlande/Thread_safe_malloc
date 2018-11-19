# Thread_safe_malloc by De Lan

## Usage
make

cd thread_tests

make

#### Then these generated executable programs can verify the efficiency and accuracy of hread_safe_malloc



### Implementation of thread-safe malloc library

In this project, I implemented two different thread-safe versions (i.e. safe for concurrent access by different threads of a process) of the malloc() and free() functions. Both of the thread-safe malloc and free functions use the best fit allocation policy.

- In version 1 of the thread-safe malloc/free functions, I used lock-based synchronization to prevent race conditions that would lead to incorrect results. These functions are to be named:
```sh
     //Thread Safe malloc/free: locking version
     void *ts_malloc_lock(size_t size);
     void ts_free_lock(void *ptr);
```
