#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct node
{
   void *data;
   struct node *next;
};

struct list
{
   struct node *head;
};

void
list_delete_at_index(struct list *list, int idx)
{
   struct node *n = list->head, **prev = &list->head;
   int i = 0;

   while (n && (i++) != idx)
   {
      prev = &n->next;
      n = *prev;
   }

   if (n)
   {
      *prev = n->next;
      free(n->data);
      free(n);
   }
}

void
list_prepend(struct list *list, struct node *n)
{
   n->next = list->head;
   list->head = n;
}

int main()
{
   struct list list = {NULL};
   struct node *n = NULL;

   srand(time(NULL));

   for (int i=10; i >= 0; --i)
   {
      n = calloc(1, sizeof(*n));
      if (!n)
      {
         perror("calloc");
         return 1;
      }
      n->data = malloc(sizeof(int));
      if (!n->data)
      {
         perror("malloc");
         return 1;
      }
      int *ip = n->data;
      *ip = i;
      list_prepend(&list, n);
   }

   list_delete_at_index(&list, rand() % 12);

   n = list.head;
   while (n)
   {
      struct node *next = n->next;
      printf("%d\n", *(int*)n->data);
      free(n->data);
      free(n);
      n = next;
   }

   return 0;
}
