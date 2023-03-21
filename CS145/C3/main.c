#include <stdio.h>
#include "C3.h"

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