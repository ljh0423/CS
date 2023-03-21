#include <iostream>
#include "vector.h"

int main() {
  cs246e::vector v;
  v.push_back(2);
  v.push_back(4);
  v.push_back(6);
  const cs246e::vector &v1 = v;
  for (auto i=v1.rbegin();i!=v1.rend();++i){
    std::cout<<*i<<std::endl;
  }
  auto a = v.rbegin();
  std::cout << (*a!=*(++a));
}