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
//find interates through the items in the linked list to find the word corresponding to the input int c. temp is mutated as it iterates through the linked list
char *find(int c, struct node *list){
  struct node *temp = list;
  while (temp!=NULL){
    if (c == temp->n) {return temp->w;}
    temp = temp->next;
  }
  return NULL;
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
  int i;
  struct node *lst = NULL; //declare the start of linked list
  while (c != EOF){ //if the first character is not EOF,
    if (c >= 'A' && c <= 'z'){ //if the first character is alphabetic
      scanf("%s", wd); //read in the first word to wd
      lst = cons(wd, cnt, lst); //add the word to the list with unique number
      printf("%s", wd);// print out the word
      cnt++; //increase counter by 1
    }
    else if (c>='0' && c<='9') { //if the first char is numeric,
      scanf("%d", &i); //read in the first number to i
      printf("%s",find(i,lst)); //print the corresponding word in the list
    }
    else { //if the first char is not alphabetic or numeric, print it out
      c = getchar();
      printf("%c",c);
    }
    c = peekchar(); //mutate c to the next first char
  }
  freelst(lst); //free all allocated strings and nodes
  return 0;
}