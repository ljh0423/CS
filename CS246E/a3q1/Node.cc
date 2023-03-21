#include <memory>
#include "Node.h"

Node &Node::operator=(const Node &other){
  if(this==&other) return *this;
  data = other.data;
  std::unique_ptr<Node> nl(other.left? new Node(*other.left) : nullptr);
  std::unique_ptr<Node> nr(other.right? new Node(*other.right) : nullptr);
  delete left;
  left = nl.release();
  delete right;
  right = nr.release();
  return *this;
}
