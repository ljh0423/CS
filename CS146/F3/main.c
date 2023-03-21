// This file is provided in support of the CS 146 W2021 final assessment.
// All other uses are prohibited unless permission is explicitly granted.
// Republication in any setting by unauthorized parties is prohibited.
//
// Author:  Brad Lushman
// Date:  April 9, 2021

// Place your solution to problem 3 in this file.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parsefr.h"

enum Conttype {MT = 0, APPL, APPR, BINL, BINR};

struct Cont{ //define struct for continuations
  enum Conttype type;
  union{
    struct AppL *al;
    struct AppR *ar;
    struct BinL *bl;
    struct BinR *br;
  };
};

struct AppL {
  struct Cont c;
  struct FRAST ast;
};

struct AppR {
  struct Cont c;
  struct FRAST val;
};

struct BinL {
  char op;
  struct Cont c;
  struct FRAST ast;
};

struct BinR {
  char op;
  struct Cont c;
  struct FRAST val;
};

void trampoline(void *(*f)()) {
  while (f) {
    void *g = f();
    f = (void *(*)())g;
  }
}

void *interp(struct FRAST f, struct Cont cont);

//subst substitutes all occurences of v in fun with the reduced value of arg
struct FRAST subst(struct FRAST fun, struct FRAST arg, char *v){
  struct FRAST newf;
  switch (fun.type){
    case 0:
      return fun;
    case 1: //if fun is app, apply subst on the body of function in app, and return it
      fun.a->fn.f->body = subst(fun.a->fn.f->body, arg, v);
      return fun;
    case 2: //if fun is binary, subst into both arg1 and arg2
      fun.b->arg1 = subst(fun.b->arg1, arg, v);
      fun.b->arg2 = subst(fun.b->arg2, arg, v);
      return fun;
    case 3: //if fun is var, if name of the var is the same as v, create new number FRAST as a copy of arg
      if (0 == strcmp(v, fun.v->name)){
        newf.type = NUMBER;
        struct Num *newn = malloc(sizeof(struct Num));
        newn->val = arg.n->val;
        freeFR(fun); //free original var FRAST
        newf.n = newn;
        return newf;
      }
      else return fun; //if name is not equal, return the original fun
    case 4:
      return fun; //if fun is num, return itself
  }
}

//applyCont apply continuation step for the cont input
void *applyCont(struct Cont ct, struct FRAST v) {
  struct Cont c1;
  struct FRAST a1;
  switch(ct.type){
    case 0: //if cont is MT, print v and newline
      printFR(v);
      printf("\n");
      freeFR(v);
      return NULL; //return NUll to end
    case 1: //if cont is AppL
      a1 = ct.al->ast; //store the ast of appl in a1
      c1.type = APPR; //create new Cont AppR with c from appl and v
      struct AppR *newa = malloc(sizeof(struct AppR));
      newa->c = ct.al->c;
      newa->val = v;
      c1.ar = newa;
      free(ct.al); //free the original Cont Appl
      return interp(a1, c1);
    case 2: //if cont is AppR
      a1 = subst(ct.ar->val.f->body, v, ct.ar->val.f->var); //substitute the current variable
      c1 = ct.ar->c; //store the continuation of appr
      freeFR(v); //free the FRAST v used in substitution
      free(ct.ar->val.f); //free the function in appr
      free(ct.ar); //free the cont appr
      return interp(a1, c1);
    case 3: //if cont is BinL
      a1 = ct.bl->ast; //store the ast of binl
      c1.type = BINR; //create new Cont BinR with op, c in binl and v
      struct BinR *newb = malloc(sizeof(struct BinR));
      newb->op = ct.bl->op;
      newb->c = ct.bl->c;
      newb->val = v;
      c1.br = newb;
      free(ct.bl); //free the original binl
      return interp(a1, c1);
    case 4: //if cont is BinR
      a1.type = NUMBER; //create new FRAST to store the result of binary operation
      struct Num *newn = malloc(sizeof(struct Num));
      switch (ct.br->op){ //for each bin, perform the corresponding binary operation with the val of number in val of BinR and v, to store in newn
        case '+':
          newn->val = ct.br->val.n->val + v.n->val;
          break;
        case '*':
          newn->val = ct.br->val.n->val * v.n->val;
          break;
        case '-':
          newn->val = ct.br->val.n->val - v.n->val;
          break;
        case '/':
          newn->val = ct.br->val.n->val / v.n->val;
          break;
      }
      a1.n = newn;
      c1 = ct.br->c;
      freeFR(ct.br->val); //free original FRAST num in binr
      freeFR(v); //free original FRAST num
      free(ct.br); //free the input binr
      return applyCont(c1, a1);
  }
}

//interp interprets the input FRAST with cont
void *interp(struct FRAST f, struct Cont cont) {
  struct Cont c1;
  struct FRAST a1;
  switch(f.type) {
    case 1: //if f is App
      a1 = f.a->fn; //store function in app to a1
      c1.type = APPL; //create new Cont AppL with arg of app and cont
      struct AppL *newa = malloc(sizeof(struct AppL));
      newa->c = cont;
      newa->ast = f.a->arg;
      c1.al = newa;
      free(f.a); //free the input App
      return interp(a1, c1);
    case 2: //if f is Bin
      a1 = f.b->arg1; //store arg1 of bin to a1
      c1.type = BINL; //create new Cont BinL with op,arg2,and cont
      struct BinL *newb = malloc(sizeof(struct BinL));
      newb->op = f.b->op;
      newb->c = cont;
      newb->ast = f.b->arg2;
      c1.bl = newb;
      free(f.b); //free the input Bin
      return interp(a1, c1);
    default: //for other f, bounce to applyCont
      return applyCont(cont, f);
  }
}

int main() {
  struct FRAST expr = parseFR(stdin);
  struct Cont cnt;
  cnt.type = MT;
  trampoline(interp(expr, cnt));
}