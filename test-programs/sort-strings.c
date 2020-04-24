#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

static int compare_strings(const void *a, const void *b)
{
   const char *i = *(const char**)a, *j = *(const char **)b;
   return strcmp(i, j);
}

static int nomem()
{
   fputs("No memory\n", stderr);
   return 1;
}

int main()
{
   char **arr = NULL;
   size_t capacity = 8;
   size_t n = 0;
   char buf[64];

   arr = malloc(capacity * sizeof(*arr));
   if (!arr)
      return nomem();

   while(fgets(buf, sizeof(buf), stdin))
   {
      if (n == capacity)
      {
         if (SIZE_MAX / (2 * sizeof(*arr)) < capacity)
            return nomem();
         capacity *= 2;
         void *p = realloc(arr, capacity * sizeof(*arr));
         if (!p)
            return nomem();
         arr = p;
      }
      size_t len = strlen(buf) + 1;
      if (len >= 2 && buf[len-2] == '\n')
      {
         buf[len-2] = 0;
         --len;
      }
      arr[n] = malloc(len);
      if (!arr[n])
         return nomem();
      memcpy(arr[n], buf, len);
      ++n;
   }

   qsort(arr, n, sizeof(*arr), compare_strings);

   for (int i=0; i<n; ++i)
   {
      printf("%s\n", arr[i]);
      free(arr[i]);
   }

   free(arr);
   return 0;
}
