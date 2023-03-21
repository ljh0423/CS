#include <stdio.h>
#include "C5.h"

void regular(int n){
  //iterate from 1 to input increasing by 1
  for (int a=1;a<=n;++a){
    int c = a;
    //for each value of a, keep dividing by 2,3,and 5 until it is not divisible
    while (c%2 == 0) c /= 2;
    while (c%3 == 0) c /= 3;
    while (c%5 == 0) c /= 5;
    //if the produced value is 1, its factors are only from 2,3,and 5
    if (c==1) printf ("%d\n",a);
  }
}