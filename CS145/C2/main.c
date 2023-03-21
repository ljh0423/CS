#include <stdio.h>
#include <ctype.h>

int peekchar() {
  int c = getchar();
  return (c == EOF) ? EOF : ungetc(c, stdin);
}

int getIntHelper(int acc) {
  char c = peekchar();
  int val = acc;
  if (c>='0' && c<='9') val = getIntHelper(16 * acc + getchar()-'0');
  else if (c>='a' && c<='f') val = getIntHelper(16 * acc + getchar()-'a'+ 10);
  else if (c>='A' && c<='F') val = getIntHelper(16 * acc + getchar()-'A'+ 10);
  return val;
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

int c1(int sum) {
  int num = getInt();
  return (num != -1) ? c1(sum + num) : sum;
}

int main(){
  printf ("%x\n", c1(0));
  return 0;
}