#include <stdio.h>

struct Node {
    int data;
    struct Node *next;
};

struct Node *reverse(struct Node *lst){
  struct Node *next = NULL;
  struct Node *pl = NULL;
  struct Node *cl = lst;
  while (cl != NULL) {
      next = cl->next;
      cl->next = pl;
      pl = cl;
      cl = next;
  }
  return pl;
}
