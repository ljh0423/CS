#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "C12.h"

//creates a hash with size s, and a pointer to an array of pointers. Null is assigned as an initial placeholder for each value of array.
struct hash make_table(int s){
  struct hash ht;
  ht.size = s;
  struct anode **tb = calloc(s,sizeof(struct anode*));
  for (int i=0;i<s;i++){
    tb[i] = NULL;
  }
  ht.table = tb;
  return ht;
}

//finds the value of key k in the hash T. Finds the pointer in the table that the key belongs to, and iterates through the linked anodes to search for key k. If k is not present in hash T, NULL is returned.
char *search(struct hash T, int k){
  int mk = k%T.size;
  struct anode *cur = T.table[mk];
  while(cur != NULL){
    if (cur->key == k) return cur->value;
    cur = cur->next;
  }
  return NULL;
}

//adds a new key value pair to the hash T, and modifies the value of key k if key k already exists in hash T. Finds the pointer in the table that the key k belongs to, and creates a new anode with the current pointer as the next parameter of the new anode. Copy of the value is made using allocation.
void add(struct hash T, int k, char *v){
  char *c = search(T,k);
  struct anode *cur = T.table[k%T.size];
  if (c == NULL){
    struct anode *newn = malloc(sizeof(struct anode*));
    newn->key = k;
    char *copy = malloc(strlen(v) + 1); 
    strcpy(copy, v);
    newn->value = copy;
    newn->next = cur;
    T.table[k%T.size] = newn;
  }
  else{
    while(cur != NULL){
      if (cur->key == k) {
        char *copy = malloc(strlen(v) + 1); 
        strcpy(copy, v);
        free(cur->value);
        cur->value = copy;
        break;
      }
      cur = cur->next;
    }
  }
}

//iterates through each linked anodes in the array table, and for each linked anodes, frees the value parameter and then the anode, iterating through all anodes using mutation of cur. The table is freed at the end.
void free_table(struct hash T){
  for(int i=0;i<T.size;i++){
    struct anode *cur = T.table[i];
    while(cur != NULL){
      struct anode *temp = cur->next;
      free(cur->value);
      free(cur);
      cur = temp;
    }
  }
  free(T.table);
}

//deletes the anode with key k in hash T if it exists. The linked anode is linked excluding the anode with key k, and the anode with key k is freed, as well as the value of that anode.
void delete(struct hash T, int k){
  int mk = k%T.size;
  struct anode *cur = T.table[mk];
  struct anode *last = cur;
  while(cur != NULL){
    if (cur->key == k) {
      if (last == cur) { //if the anode with key k is the first anode, put the next of that anode as a pointer in the table
        T.table[mk] = cur->next;
        free(cur->value);
        free(cur);
        break;
      }
      else{
        last->next = cur->next;
        free(cur->value);
        free(cur);
        break;
      }
    }
    if (last != cur) last = cur;
    cur = cur->next;
  }
}
