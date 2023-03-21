// This file is provided in support of the CS 146 W2021 final assessment.
// All other uses are prohibited unless permission is explicitly granted.
// Republication in any setting by unauthorized parties is prohibited.
//
// Author:  Brad Lushman
// Date:  April 9, 2021

// Place your solution to problem 1 here.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parsesimpl.h"

//find finds the assignmed value for the id input in the environment of decls
int find(struct DeclListNode *lst, char id[]){
  if (0 == strcmp(lst->decl.var, id)) {
    return lst->decl.val;
  }
  else return find(lst->next, id);
}

//for each aexp types, evaluates to a value using the environment
int aexprc(struct AEAST exp, struct DeclListNode *env){
  switch (exp.type){
    case 0 : //for binary operations perform the corresponding operation with recursion on the operators
      switch (exp.b->op){
        case '+' :
          return aexprc(exp.b->arg1, env) + aexprc(exp.b->arg2, env);
        case '*' :
          return aexprc(exp.b->arg1, env) * aexprc(exp.b->arg2, env);
        case '-' :
          return aexprc(exp.b->arg1, env) - aexprc(exp.b->arg2, env);
        case '/' :
          return aexprc(exp.b->arg1, env) / aexprc(exp.b->arg2, env);
        case '%' :
          return aexprc(exp.b->arg1, env) % aexprc(exp.b->arg2, env);
      }
    case 1 : //for numerical values produce the value itself
      return exp.n->val;
    case 2 : //for variables, use find to find the corresponding value and return
      return find(env, exp.i->name);
  }
}

//for each bexp types, evaluates to a value using the environment
int bexprc(struct BEAST exp, struct DeclListNode *env){
  switch(exp.type) {
    case 0 : //for comparisons, perform the corresponding comparison with aexprc for arguments
      if (0 == strcmp(exp.c->op, "=")) return aexprc(exp.c->arg1, env) == aexprc(exp.c->arg2, env);
      else if (0 == strcmp(exp.c->op, ">")) return aexprc(exp.c->arg1, env) > aexprc(exp.c->arg2, env);
      else if (0 == strcmp(exp.c->op, "<")) return aexprc(exp.c->arg1, env) < aexprc(exp.c->arg2, env);
      else if (0 == strcmp(exp.c->op, ">=")) return aexprc(exp.c->arg1, env) >= aexprc(exp.c->arg2, env);
      else if (0 == strcmp(exp.c->op, "<=")) return aexprc(exp.c->arg1, env) <= aexprc(exp.c->arg2, env);
    case 1 : //return not of the bexp arg
      return !bexprc(exp.n->arg, env);
    case 2 : //return and of two bexp arguments
      return bexprc(exp.a->arg1, env)&&bexprc(exp.a->arg2, env);
    case 3 : //return or of two bexp arguments
      return bexprc(exp.o->arg1, env)||bexprc(exp.o->arg2, env);
    case 4 : //return the true/false value
      return exp.l->val;
  }
}

//mutates the environment by changing the value for the input id
void chgenv(struct DeclListNode *env, char id[], int v){
  if (0 == strcmp(env->decl.var, id)){
    env->decl.val = v;
  }
  else chgenv(env->next, id, v);
}

//for each statement type, performs corresponding action
void pcsprog(struct StmtAST stmt, struct DeclListNode *env){
  switch (stmt.type){
    case 0 : break; //skip does nothing
    case 1 : //set mutates using the chgenv function
      chgenv(env, stmt.set->var, aexprc(stmt.set->expr, env));
      break;
    case 2 : //seq recursively calls both stmts in seq
      pcsprog(stmt.seq->stmt1, env);
      pcsprog(stmt.seq->stmt2, env);
      break;
    case 3 : //print prints the value of aexp
      printf("%d\n", aexprc(stmt.print->expr, env));
      break;
    case 4 : //if is interpreted to if using bexp and recursion on either t/fstmt
      if (bexprc(stmt.iif->test, env)) pcsprog(stmt.iif->tstmt, env);
      else pcsprog(stmt.iif->fstmt, env);
      break;
    case 5 : //while is interpreted to while using bexp and recursion on body of the loop
      while (bexprc(stmt.loop->test, env)) {
        pcsprog(stmt.loop->body, env);
      }
      break;
  }
}

int main() {
  struct Program prog = parseProgram(stdin); //parses the program from input
  pcsprog(prog.stmt, prog.decls); //interprets the given simpl program
  freeProgram(&prog); //frees all allocated memory in parsed program
  return 0;
}