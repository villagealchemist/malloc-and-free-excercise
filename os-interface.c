#include "os-interface.h"

static size_t align(size_t size, size_t offset)
{
   return (size + offset - 1) / offset * offset;
}

#if defined(_WINDOWS) || defined(_MSC_VER)

//
// If anybody out there is using Windows, this is how to ask the OS for memory.
//

#include <windows.h>

void *os_allocate_memory(size_t *size)
{
   *size = align(*size, 4096);

   return VirtualAlloc(NULL, *size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
}

void os_return_memory(void *ptr, size_t size)
{
   VirtualFree(ptr, size, MEM_RELEASE);
}

#else

//
// Typical modern Unix way to get more memory - using mmap(2)
//

#include <sys/mman.h>

#ifndef PAGE_SIZE
#define PAGE_SIZE 4096
#endif

void *os_allocate_memory(size_t *size)
{
   *size = align(*size, PAGE_SIZE);

   void *r = mmap(NULL, *size, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANON, -1, 0);
   if (r == MAP_FAILED)
      r = NULL;
   return r;
}

void os_return_memory(void *ptr, size_t size)
{
   munmap(ptr, size);
}

#if 0

//
// Commented out.  A more ancient/traditional Unix interface.
//

void *os_allocate_memory(size_t *size)
{
   // ISSUE: on BSD, sbrk() uses an int, so we shoulch compare *size with
   // INT_MAX.
   // But on Linux sbrk uses intptr_t so we're fine ...

   void *r = sbrk(*size);
   if (r == (void)~0U)
      r = NULL;
   return r;
}

void os_return_memory(void *ptr, size_t size)
{
   // Uhhh.  This is kinda hard to do with sbrk.
}

#endif

#endif
