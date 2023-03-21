#include <stdio.h>
#include "C10.h"
void printNums(int m, int n){
  int div=1, k=m, c=2;
  goto L11;
L11: //initialize variables for checking for palindrome
  div=1;
  k=m;
  goto L;
//from L and L0 cycle, it calculates the largest divisor in the form 10^n for m
L:
  if (m/div >= 10) goto L0;
  goto L1;
L0:
  div *= 10;
  goto L;
L1:
  if (m > n) return;
  if (div <= 1) goto L22; //if the number is 1 digit, then it is palindromic
  if (k/div == k%10) goto L2; //if the first and last digits are equal
  goto L4; //if the number is not a palindrome, skip to next value of m
L2: //continue checking without the first and last digit
  k = (k % div)/10;
  div /= 100;
  goto L1;
L22: //initialize variables to check if m is square free
  c=2;
  goto L5;
L5:
  if (m%(c*c) == 0) goto L4; //if m is not square free, then skip to next m
  if (m-(c*c) >= (2*c + 1)) goto L6; //if the next c^2 is not greater than m, increase c by 1
  goto L7; //if m is a palindrome and square-free, print m
L6:
  c++;
  goto L5;
L7:
  printf("%d\n", m);
  goto L4;
L4: //increase m by 1 if m+1 is less than or equal to n
  if (n-m < 0) return;
  m++;
  goto L11;
}
int main(){
  printNums(1,100000);
  return 0;
}