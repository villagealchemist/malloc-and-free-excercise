#include <stdio.h>   // for fgetc, stdin, fputs, stdout
#include <stdlib.h>  // for malloc, realloc, free

// API conforms to popular GNU readline library.  We return a buffer on
// the heap, or NULL on failure.
//
char *
readline(const char *prompt)
{
   // User can specify a string that can get spit out as a prompt, as in
   // the '$ ' that the Unix shell might give you.  NULL means a default.
   //
   fputs(prompt ? prompt : "> ", stdout);

   // Flush the prompt to stdout.  The default is for the FILE* object to
   // keep it buffered until the next newline, but the prompt will not likely
   // have a newline.
   //
   fflush(stdout);

   char *buf = NULL;
   size_t len = 0;
   size_t capacity = 0;

   int c;                 // Note that c must be int, not char, to distinguish
                          // between EOF and char value 0xff.

   // Give some initial buffer size.
   //
   capacity = 8;
   buf = malloc(capacity);
   if (!buf)
      return NULL;

   while ((c = fgetc(stdin)) != EOF && c != '\n')  // read until EOF or newline
   {
      // Do we have enough space for current char plus null terminator?
      //
      if ((capacity-len) < 2)
      {
         capacity *= 2;
         void *p = realloc(buf, capacity);
         if (!p)
         {
            // Realloc failed, free old buffer and bail.
            //
            free(buf);
            return NULL;
         }
         buf = p;
      }

      // Add char.
      //
      buf[len++] = c;
   }

   // Add null terminator.
   // Note: This is OK because we guaranteed extra space for it on lines 30&39 
   //
   buf[len] = 0;

   // For EOF, we want to return NULL rather than the empty string to terminate
   // the loop.
   //
   if (!*buf && c == EOF)
   {
      free(buf);
      buf = NULL;
   }

   return buf;
}

int main()
{
   char *p = NULL;
   while ((p = readline(NULL)))
   {
      printf("You wrote: %s\n", p);
      free(p);
   }
   return 0;
}
