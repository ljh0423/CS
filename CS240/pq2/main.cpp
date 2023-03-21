#include <iostream>
#include "list.h"

int main() {
  cs246e::list l;
  l.push_front(2).push_front(3);
  auto it = l.begin();
  ++it;
  l.insert(it, 6);

  for(auto i=l.begin();i!=l.end();++i){
    std::cout << *i <<std::endl;
  }
}