#include <iostream>
#include "map.h"

int main() {
  cs246e::map<int,int> m1;
  cs246e::map<int,int> m2 {};
  std::cout<< m1[1]<<std::endl;
  std::cout<< m1[2]<<std::endl;
  std::cout<< m1[4]<<std::endl;
  m2 = m1;
  std::cout<< m2[1]<<std::endl;
  std::cout<<sizeof(std::pair<const int,int>);
} 