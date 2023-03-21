#include <stdio.h>
#include "C6.h"

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

void prime_factors(int n){
  //if n is even and not 0, then print 2 and divide n by 2
  while (n%2==0 && n != 0){
    printf ("2\n");
    n /= 2;
  }
  //any other prime than 2 is odd, so check for factors using increments by 2, from 3 to sqrt of n
  for (int i=3;i<=isqrt(n);i+=2){
    while (n%i==0){
      printf ("%d\n", i);
      n /= i;
    }
  }
  //if the left value of n is greater than 2, it itself is a prime factor of n
  if (n > 2) printf ("%d\n",n);
}