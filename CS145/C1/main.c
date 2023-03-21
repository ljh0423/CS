#include <stdio.h>
#include <ctype.h>

int peekchar() {
  int c = getchar();
  return (c == EOF) ? EOF : ungetc(c, stdin);
}

int getIntHelper(int acc) {
  char c = peekchar();
  return (c>='0' && c<='9') ? getIntHelper(10 * acc + getchar()-'0'): acc;
}

void skipwhitespace() {
  if (isspace(peekchar())) {
    getchar();
    skipwhitespace();
  }
}

int getInt() {
  skipwhitespace();
  return (peekchar() == EOF) ? -1 : getIntHelper(0);
}

int c1() {
  int num = getInt();
  return (num != -1) ? num+c1() : 0;
}

int main(){
  printf ("%d\n", c1());
  return 0;
}