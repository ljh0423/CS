// This file is provided in support of the CS 146 W2021 final assessment.
// All other uses are prohibited unless permission is explicitly granted.
// Republication in any setting by unauthorized parties is prohibited.
//
// Author:  Brad Lushman
// Date:  April 9, 2021

// Place your solution to problem 2 in this file.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parsefr.h"

struct FRAST interp(struct FRAST exp);

struct FRAST subst(struct FRAST fun, struct FRAST arg, char *v){
  struct FRAST newf;
  switch (fun.type){
    case 0:
      return fun;
    case 1: //if fun is app, apply subst on the body of function in app, then return it
      fun.a->fn.f->body = subst(fun.a->fn.f->body, arg, v);
      return fun;
    case 2: //if fun is bin, subst into both arg1 and arg2
      fun.b->arg1 = subst(fun.b->arg1, arg, v);
      fun.b->arg2 = subst(fun.b->arg2, arg, v);
      return fun;
    case 3: //if fun is var, if name of the var is the same as v, create new number FRAST as a copy of arg
      if (0 == strcmp(v, fun.v->name)){
        newf.type = NUMBER;
        struct Num *newn = malloc(sizeof(struct Num));
        newn->val = arg.n->val;
        freeFR(fun);
        newf.n = newn;
        return newf;
      }
      else return fun; //if name is not equal, return the original fun
    case 4:
      return fun; //if fun is num, return itself
  }
}

//interp interprets the FRAST input to a final value
struct FRAST interp(struct FRAST exp){
  struct FRAST newf;
  struct FRAST ar;
  switch (exp.type){
    case 0: //function by itself does nothing
      return exp;
    case 1: //substitute arg into body of the function in app and interp the result
      ar = interp(exp.a->arg);
      newf = interp(subst(exp.a->fn.f->body, ar, exp.a->fn.f->var));
      free(exp.a->fn.f);
      free(exp.a);
      freeFR(ar);
      return newf;
    case 2: //create a new FRAST of type number, and store the result of the input exp
      newf.type = NUMBER; //declare newf as a num
      struct Num *newn = malloc(sizeof(struct Num));
      struct FRAST a1 = interp(exp.b->arg1); //interp arg1 to a value
      struct FRAST a2 = interp(exp.b->arg2);//interp arg2 to a value
      switch (exp.b->op){ //for each bin, perform the corresponding binary operation with the resulting values of arg1 and arg2 and store in newn
        case '+':
          newn->val = a1.n->val + a2.n->val;
          break;
        case '*':
          newn->val = a1.n->val * a2.n->val;
          break;
        case '-':
          newn->val = a1.n->val - a2.n->val;
          break;
        case '/':
          newn->val = a1.n->val / a2.n->val;
          break;
      }
      newf.n = newn; //put newn in newf as num
      free(exp.b);
      freeFR(a1);
      freeFR(a2);
      return newf;
    case 3: //if exp is var return itself
      return exp;
    case 4: //if exp is num, return itself
      return exp;
  }
}

int main() {
  struct FRAST expr = parseFR(stdin); //parse from input
  struct FRAST res = interp(expr); //interpret the parsed expr to a value
  printFR(res);// print the resulting value
  printf("\n");
  freeFR(res);//free the result
}