#include <stdio.h>
#include "array.h"

void getl(char c, int a3[]);

int main(void) {
  int a3[]={0,0,0};
  char c;
  while(1){
    c  = getchar();
    if (c == EOF) return 0;
    getl(c, a3);
    //pp();
  }
  return 0;
}

void getl(char c, int a3[]){
  static int a1=0,a2=0;
  int a, b;
  if(c == 'u'){
    scanf("%d %d", &a, &b);
    if (a == 0){
      if (a3[1] != 0){
        put((a3[0] + a3[1]), get(a3[0]));
        a3[0] += 1;
        a3[2] = (a3[2] + 1)%a3[1];
      }
      put(a1, b);
      ++a1;
    }
    else if (a == 1){
      put((ArraySize - 1 - a2), b);
      ++a2;
    }
    else {
      if (a3[2] != 0){
        for(int i=0;i<a3[2];i++){
          put(a3[0] + a3[1] - i, get(a3[0] + a3[1] - 1 - i));
        }
        put(a3[0] + a3[1] - a3[2], b);
        a3[1] += 1;
      }
      else if (a3[1] == 0){
        put(a1, b);
        a3[0] = a1;
        a3[1] += 1;
        a3[2] = 0;
      }
      else {
        put((a3[0] + a3[1]), b);
        a3[1] += 1;
      }
    }
  }
  else if (c == 'o') {
    scanf("%d", &a);
    if (a == 0){
      --a1;
      printf("%d\n", get(a1));
      if (a3[1] != 0){
        put(a3[0] - 1, get(a3[0] + a3[1] - 1));
        a3[0] -= 1;
        a3[2] = (a3[2]-1)%a3[1];
        a3[2] = (a3[1] + (a3[2] % a3[1])) % a3[1];//get positive mod
      }
    }
    else if (a == 1){
      printf("%d\n", get(ArraySize - a2));
      --a2;
    }
    else {
      if (a3[2] != 0){
        printf("%d\n", get(a3[0] + a3[1] - a3[2] - 1));
        for(int c=a3[2];c>0;c--){
          put((a3[0] + a3[1] - c - 1), get(a3[0] + a3[1] - c));
        }
        a3[1] -= 1;
        if(a3[2] == a3[1]) a3[2] = 0;
      }
      else {
        printf("%d\n", get(a3[0] + a3[1] - 1));
        a3[1] -= 1;
      }
    }
  }
}