#include <stdio.h>
#include <stdlib.h>
#include "C9.h"

//this function print the number represented in nlst, prints the number in order from inside to outside recursively. ex.print_num(nlst)->runs print_num(nlst->next) and prints the bigit of nlst after.
void print_num(struct Node *nlst){
  if (nlst != NULL){
    if (nlst->next==NULL){
      printf ("%d", nlst->bigit);
    }
    else {
      print_num(nlst->next);
      printf ("%04d", nlst->bigit);
    }
  }
  else if (nlst == NULL) printf("0");
}

//frees all allocated memory in the input node blst. Iterates through each linked nodes to free each of them. Mutates blst as it iterates through by replaceing it with next. ex.free_num({a, next})->frees current node, set blst to next, and free blst until blst==NULL.
void free_num(struct Node *blst) {
  struct Node *temp;
  while (blst!=NULL) {
    temp = blst->next;
    free(blst);
    blst = temp;
  }
}

//creates a new node with bgt as bigit and nxt as next. Uses malloc to allocate memory for the new node. ex.cons_bigit(bgt, nxt)->returns pointer to node {bgt, nxt}
struct Node *cons_bigit(int bgt, struct Node *nxt){
  struct Node *newn = malloc(sizeof(struct Node));
  newn->bigit = bgt;
  newn->next = nxt;
  return newn;
}

//this function creates a copy of nlst, in a separate space so that the original and the copy do not affect each other. Mutates nlst as it iterates through the linked node nlst. ex.copy_num(nlst)->creates and returns pointer to node newn, representing the same integer as nlst, by allocating it in different memory.
struct Node *copy_num(struct Node *nlst){
  struct Node *newn = NULL;
  struct Node *temp, *last = NULL;
  while (nlst != NULL){
    temp = cons_bigit(nlst->bigit, NULL);
    if (newn == NULL){
      newn = temp;
    }
    else {
      last->next = temp;
    }
    nlst = nlst->next;
  }
  return newn;
}

//adds two integers represented as linked nodes, n1lst and n2lst. The result of addition is represented as linked nodes with bigits. Mutates n1lst and n2lst as it iterates through the linked node nlst. ex.add(n1lst, n2lst)->returns pointer to new node newn, representing the sum of n1lst and n2lst.
struct Node *add(struct Node *n1lst, struct Node *n2lst){
  int a,d=0;
  struct Node *newn = NULL;
  struct Node *temp, *last = NULL;
  while (n1lst!=NULL || n2lst!=NULL){
    a = (n1lst ? n1lst->bigit : 0) + (n2lst ? n2lst->bigit : 0) + d;
    d = a/10000;
    a = a%10000;
    temp = cons_bigit(a, NULL);
    if (newn == NULL){
      newn = temp;
    }
    else {
      last->next = temp;
    }
    last = temp;
    if (n1lst) n1lst = n1lst->next;
    if (n2lst) n2lst = n2lst->next;
  }
  if (d>0){
    temp->next = cons_bigit(d, NULL);
  }
  return newn;
}

//helper function for mult, to multiply a bigit n2, with linked node n1lst. Mutates n1lst as it iterates through the linked node n1lst. ex.helpmult(n1lst, n2)->creates and returns pointer to new node newn, which representes n1lst multiplied by int n2.
struct Node *helpmult(struct Node *n1lst, int n2){
  int a,d=0;
  struct Node *newn = NULL;
  struct Node *temp, *last = NULL;
  while (n1lst!=NULL){
    a = (n1lst->bigit * n2) + d;
    d = a/10000;
    a = a%10000;
    temp = cons_bigit(a, NULL);
    if (newn == NULL){
      newn = temp;
    }
    else {
      last->next = temp;
    }
    last = temp;
    if (n1lst) n1lst = n1lst->next;
  }
  if (d>0){
    temp->next = cons_bigit(d, NULL);
  }
  return newn;
}

//multiplies two linked nodes n1lst and n2lst, while it mutates n1lst and n2lst as it iterates through the linked node n2lst. For each bigit in n2lst, it gets multiplied to n1lst, and added back together. ex.mult(n1lst, n2lst)->creates and returns pointer to new node newn, representing the multiplied integer between n1lst and n2lst.
struct Node *mult(struct Node *n1lst, struct Node *n2lst){
  if (n1lst==NULL) return NULL;
  if (n2lst==NULL) return NULL;
  int c=0;
  struct Node *newn = NULL;
  struct Node *temp;
  while (n2lst != NULL){
    temp = helpmult(n1lst, n2lst->bigit);
    for (int i=0;i<c;i++){
      temp = cons_bigit(0, temp);
    }
    if (newn == NULL) newn = temp;
    else {
      struct Node *n = newn;
      newn = add(newn, temp);
      free_num(n);
      free_num(temp);
    }
    n2lst = n2lst->next;
    ++c;
  }
  return newn;
}
