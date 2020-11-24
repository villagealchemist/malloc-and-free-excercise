#include <stddef.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>

#include "os-interface.h"



struct memory_block
{
   size_t size;

   struct memory_block *next;

   char mem[];
};

struct list
{
   struct memory_block *head;
};

struct list freelist = {};


void
free_list_prepend(struct list *list, struct memory_block *n)
{
   n->next = list->head;
   list->head = n;
}


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

   size_t blockSize = n + BLOCK_HEADER_SIZE;
   struct memory_block *cur = freelist.head;
   struct memory_block *prev;
   while (cur)
   {
      if (cur->size >= blockSize)
      {
         struct memory_block *next = cur->next;
         size_t newSize = cur->size - n;
         mem = cur;
         mem->size = n;
         if (cur == freelist.head)
         {
            cur = cur + blockSize;
            freelist.head = cur;
         }
         else
         {
            cur = cur + blockSize;
            prev->next = cur;
         } 
         cur->next = next;
         cur->size = newSize;
      }
      prev = cur;
      cur = cur->next;
   }
   
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

   if(!freelist.head)
   {
      freelist.head = mem;
   }
   else
   {
      struct memory_block *cur = freelist.head;
      struct memory_block *prev;
      int added = 0;

      while (cur && added == 0)
      {
         size_t memTotal = mem->size + BLOCK_HEADER_SIZE;
         size_t curTotal = cur->size + BLOCK_HEADER_SIZE;
         if (mem - curTotal == cur) 
         {
            cur->size += memTotal;
            added = 1;
         }
         else if (cur - memTotal == mem)
         {   
            if(cur == freelist.head)
            {
               freelist.head = mem;
            }
            else
            {
               prev->next = mem;
            }
            mem->size += curTotal;
            mem->next = cur->next;
            added = 1;
         }
         
         prev = cur;
         cur = cur->next;
         
      }

      if(added == 0){
         free_list_prepend(&freelist, mem);
         added = 1;
      }
   }
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

