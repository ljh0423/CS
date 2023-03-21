#include <iostream>
#include <string>
#include "re1.h"
using namespace std;

int main() {
  CS246E::RegExp *re = new CS246E::Concatenation{new CS246E::Star{new CS246E::Disjunction{new CS246E::Word{"a"},new CS246E::Word {"ab"}}},new CS246E::Word{"bc"}};

  string s;
  while (getline(cin, s)) {
    cout << boolalpha << re->matches(s) << endl;
  }

  delete re;
} 