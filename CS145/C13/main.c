#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//declare struct node to store corresponding number for words as linked lists
struct node {
  char *w;
  int n;
  struct node *next;
};
//peekchar reads the first char in the input, and returns it to the stdin if not EOF
int peekchar() {
  char c = getchar();
  return (c == EOF) ? EOF : ungetc(c, stdin);
}
//cons adds to the linked lst *nxt, by allocating new node and storing the new word and number in that node along with previous list as next.
struct node *cons(char *word, int c, struct node *nxt){
  struct node *newl = malloc(sizeof(struct node));
  char *copy = malloc(strlen(word) + 1); 
  strcpy(copy, word);
  newl->w = copy;
  newl->n = c;
  newl->next = nxt;
  return newl;
}
//find interates through the items in the linked list to check if word exists in the current linked list. temp is mutated as it iterates through the linked list
int find(char *word, struct node *list){
  struct node *temp = list;
  while (temp!=NULL){
    if (0 == strcmp(temp->w, word)) {return temp->n;}
    temp = temp->next;
  }
  return -1;
}
//freelst frees all allocated heap memory for w and the node for the entire linked list
//temp and list is mutated as it iterates through
void freelst(struct node *list) {
  struct node *temp;
  while (list!=NULL) {
    temp = list->next;
    free(list->w);
    free(list);
    list = temp;
  }
}
int main(void) {
  int cnt = 0; //counter for assigning unique number for each new word
  char c = peekchar();
  char wd[81];
  struct node *lst = NULL; //declare the start of linked list
  while (c != EOF){ //if the first character is not EOF,
    if (c >= 'A' && c <= 'z'){ //if the first character is alphabetic
      scanf("%s", wd); //read in the first word to wd
      int t = find(wd, lst); //check if wd is in the current linked list
      if (-1 == t){ //if wd does not exist in lst, add wd to lst using cons, print wd, and increase counter by 1
        lst = cons(wd, cnt, lst);
        printf("%s", wd);
        cnt++;
      }
      else { //if wd exists in lst, print out the assigned value for wd
        printf("%d", t);
      }
    }
    else { //if the first char is not alphabetic, read in the character and print it out
      c = getchar();
      printf("%c", c);
    }
    c = peekchar(); //check the next first char
  }
  freelst(lst); //free all allocated strings and nodes
  return 0;
}