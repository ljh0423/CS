#include <iostream>
#include <string>
#include "re.h"

using namespace std;
int main() {
  string s;
  getline(cin, s);
  auto re = CS246E::parseDisjunction(s);
  cout << *re << endl;
} 