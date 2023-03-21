#include <iostream>
#include "deque.h"

int main() {
  cs246e::deque<int> d1 {2,3,4};
  d1.push_front(1);
  for(auto i=d1.begin();i!=d1.end();++i){
    std::cout<<*i;
  }
} 