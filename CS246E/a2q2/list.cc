#include <cstddef>
#include <iostream>
#include <utility>
#include "list.h"
namespace cs246e {

list::Node::Node(int data, Node *next): data{data}, next{next} {}

list::Node::Node(const Node &other): data{other.data}, next{other.next? new Node{*other.next}: nullptr} {}

list::Node::Node(Node &&other): data{other.data}, next{other.next} {
  other.next = nullptr;
}

list::Node &list::Node::Node::operator=(Node other) {
  data = other.data;
  std::swap(next, other.next);
  return *this;
}

list::Node::~Node() { delete next; }

list::list() {}

list::list(const list &other): theList{other.theList ? new Node{*other.theList}: nullptr}, len{other.len} {}

list::list(list &&other): theList{other.theList}, len{other.len} {
  other.theList = nullptr;
  other.len = 0;
}

list &list::operator=(list other) {
  std::swap(theList, other.theList);
  std::swap(len, other.len);
  return *this;
}

size_t list::size() const { return len; }

list &list::push_front(int n) {
  theList = new Node{n, theList};
  ++len;
  return *this;
}

list &list::pop_front() {
  if (len) {
    Node *tmp = theList;
    theList = theList->next;
    tmp->next = nullptr;
    delete tmp;
    --len;
  }
  return *this;
}

int &list::operator[](size_t i) {
  return get(i);
}

const int &list::operator[](size_t i) const {
  return get(i);
}

int &list::get(size_t i) const {
  Node *cur = theList;
  for (size_t j = 0; j < i; ++j) cur = cur->next;
  return cur->data;
}

list::~list() { delete theList; }

std::ostream &operator<<(std::ostream &out, const list &l) {
  for (auto &n: l) out << n << ' ';
  return out;
}
}
