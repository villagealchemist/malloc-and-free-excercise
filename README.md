# CS221 - C and Systems Programming
# Project 2: `malloc`

### Due date: 7 May 2020

## Synopsis

You will write a simple `malloc` and `free` implementation.

The barebones version is already there for you in `malloc.c`.  You will need to fill in pieces which are commented with `TODO`.

Implementations for `calloc` and `realloc` are also provided as convenience, but you may mostly leave them alone.  You will want to focus on finishing up `malloc` and `free`.

## File listing

* `malloc.c`.  This is where the action lives.
* `os-interface.h`, `os-interface.c`.  This is how your project will ask the operating system for memory if it cannot satisfy the request from the free list.  This code is platform-specific.  You should not have to change this code.  If you are curious to read it, there is an implementation for Windows, and 2 sample implementations for Unix [`mmap` and `sbrk`].
* `test-programs/*.c`.  These are test programs taken more or less directly from Canvas.  Code we wrote during lecture.

To build, type `make`.  You can test your solution with the test programs:

* `linkedlist-lab` creates a linked list of 11 elements, and removes a random one.
* `readline-heap` will read from standard input into a heap buffer, and spit it back out.
* `sort-strings` reads from standard input (terminate with ctrl-D) and sorts the lines on the heap

## What's missing

The barebones `malloc.c` provided to get you started has a few issues.  Each of these is mentioned with a comment saying `TODO`.

* There is no free list.  `free()` does nothing.  You need to fix that.
* `malloc` does not consult a free list, it asks the OS for more memory every time.

Also, you must:

* Inside `free`: scan the free list for physically adjacent blocks.  Merge them.
* Inside `malloc`: if you see a free list node that is larger than the `n` parameter of `malloc`, split it into multiple blocks, inserting the unused space back into the free list.

Points will be severely deducted for "memory leak" type issues.  Right now, `free()` does nothing, but it should work.  Any memory returned to you by `free()` should show up in some kind of free list structure for future use by `malloc`.

## Extra credit

Some "stretch goals" have been mentioned in comments:

* Add an optimizing `realloc` that checks if physically adjacent blocks are available, and merges it with the existing block if appropriate.
* Have `free()` return memory back to the OS when large chunks become free.  You can use `os_return_memory()` from `os-interface.h`.

Additionally, extra credit will be given for efficient algorithms for scanning the free list.  (The simplest algorithms are O(n) scans of the entire free list.  Can you get creative and make yours faster?)

Some additional suggestions for extra credit:

* Add pthread mutex protecting free list for thread safety.

* Add some kind of "magic value" to the memory block structure to attempt to detect heap corruption.  The idea is to write some constant to the memory block, and later check if it has been overwritten with another value.  This would indicate someone performed an out-of-bounds write, and the program should be stopped.

* Make sure that `->mem`'s address aligns on a 8 or 16 byte boundary.  In practice, a production implementation would want to do this because some CPUs may need memory locations to be aligned for a particular data structure, or may perform faster if they do so.

