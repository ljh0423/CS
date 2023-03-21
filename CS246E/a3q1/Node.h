#include <iostream>

struct Node {
  int data;
  Node *left, *right;
  Node(int data, Node *left,Node *right): data{data}, left{left}, right{right} {}
  Node &operator=(const Node &other);
  Node(const Node &other) {
    //deep copy
  }
};