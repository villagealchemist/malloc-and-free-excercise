#include <stddef.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>

#include "os-interface.h"

struct memory_block
{
   size_t size;

   // TODO: add additional fields (next pointer for free list?)

   // This field must appear last!
   char mem[];
};

// The amount of bytes before ->mem in a memory_block.
//
#define BLOCK_HEADER_SIZE offsetof(struct memory_block, mem)

// Given a pointer to ->mem, convert to memory_block.
//
#define PTR_TO_BLOCK(PTR) \
   ((struct memory_block*)(((char*)PTR) - BLOCK_HEADER_SIZE))

void *malloc(size_t n)
{
   struct memory_block *mem = NULL;

   // If we add the block header size to n, make sure we still fit in a size_t.
   //
   if (n > SIZE_MAX - BLOCK_HEADER_SIZE)
      return NULL;

   // TODO: consult free list for available memory.

   // TODO: if we have a free list node where ->size is much biger than n,
   //       split into multiple nodes and insert the extra nodes into the
   //       free list

   // If free list check failed, ask OS for memory
   //
   if (!mem)
   {
      size_t size = BLOCK_HEADER_SIZE + n;
      mem = os_allocate_memory(&size);
      if (mem)
      {
         mem->size = size;
      }
   }

   return mem ? mem->mem : NULL;
}

void free(void *p)
{
   // free(NULL) is a no-op.
   //
   if (!p)
      return;

   struct memory_block *mem = PTR_TO_BLOCK(p);

   // TODO: insert into free list.

   // TODO: consider merging with other physically adjacent free list nodes

   // TODO (extra credit) - if there is a large amount of free memory, give
   // it back to the OS with os_return_memory().
}

void *realloc(void *p, size_t newsize)
{
   // Input pointer is null?  Treat it as a malloc.
   //
   if (!p)
      return malloc(newsize);

   // Get the block for this pointer.
   //
   struct memory_block *mem = PTR_TO_BLOCK(p);

   // Allocation already big enough?  No work to do.
   //
   if (mem->size >= newsize)
      return p;

   // TODO (extra credit):
   // Is there something in the free list physically adjacent to
   // this block?  Maybe we could merge them and not have to move the
   // buffer.

   // Allocate a new buffer
   //
   void *q = malloc(newsize);
   if (q)
   {
      // Copy old contents into new and free old one.
      //
      memcpy(q, p, mem->size);
      free(p);
   }
   return q;
}

void *calloc(size_t n, size_t size)
{
   // Check for overflow.
   //
   if (size && n > SIZE_MAX/size)
      return NULL;

   void *p = malloc(n*size);
   if (p)
      memset(p, 0, n*size);
   return p;
} 

