#include <iostream>
#include "Node.h"

int main() {
  Node n(3,nullptr,nullptr);
  Node n1(1,nullptr,nullptr);
  Node n2(2,nullptr,nullptr);
  n.left = &n1;
  std::cout<< n.left->data;
  n.right = &n2;
  std::cout<< n.right->data;
  Node n3(3,nullptr,nullptr);
  n3 = n;
  std::cout<< (n3.left==nullptr);
}