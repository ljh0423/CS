#include <stdio.h>
#include "C4.h"

//isqrt from C3
int isqrt(int n) {
  //if the input is 0 or 1, return input
  if (n == 0 || n == 1)  return n; 
  int sqrn=1;
  //if the difference between n and current sqrn^2 is greater than (sqrn+1)^2-sqrn^2, then increase sqrn by 1
  while ((n-sqrn*sqrn) >= sqrn+sqrn+1){
    ++sqrn;
  }
  return sqrn; 
}

void sumsqr(int n){
  //assign largest integer sqrt value of n to sqrn
  int sqrn = isqrt(n);
  //assign largest integer sqrt of n/2 to st
  int st = isqrt(n/2);
  //Since the first number, i , has to be equal or greater, start at st and iterate until sqrn
  for (int i=st;i<=sqrn;++i) {
    //assign c as sqrt of (n-(i*i))
    int c = isqrt(n-(i*i));
    //check if i and c completes sumsquare
    if (c*c + i*i == n) printf ("%d %d\n",i,c);
  }
}