/*
This code construct the list class using singly linked nodes.
I have added the functionality of insert to the list class, such that insert function inserts a given integer to the given position in the list.
*/
#ifndef LIST_H
#define LIST_H
#include <cstddef>
#include <iostream>

namespace cs246e {
class list {
  struct Node {
    int data;
    Node *next;
    Node(int data, Node *next);
    Node(const Node &other);
    Node(Node &&other);
    Node &operator=(Node other);
    ~Node();
  };

  Node *theList = nullptr;
  size_t len = 0;

 public:
  list();
  list(const list &other);
  list(list &&other);
  list &operator=(list other);
  size_t size() const;
  list &push_front(int n);
  list &pop_front();
  int &operator[](size_t i);
  const int &operator[](size_t i) const;
  ~list();

  class iterator {
    Node *p;
    iterator(Node *p): p{p} {}
   public:
    bool operator!=(const iterator &other) const { return p != other.p; }
    int &operator*() const { return p->data; }
    iterator &operator++() {
      p = p->next;
      return *this;
    }
    friend class list;
  };

  iterator begin() { return iterator{theList}; }
  iterator end() { return iterator{nullptr}; }
  iterator insert(iterator posn, int x){
    int o,n=x;
    if (posn!=nullptr){
      iterator i=posn;
      //inserts x at posn, and pushes data of each node to 1 after
      for(;;++i){
        o = i.p->data;
        i.p->data = n;
        n = o;
        if(i.p->next==nullptr) break;
      }
      //create new node to accomodate the increased length of list
      Node *nn = new Node{n, nullptr};
      i.p->next = nn;
      ++len;
    }
    else { //if posn is nullptr
      if (theList==nullptr){//if the list is empty
        Node *nn = new Node{x, nullptr};
        theList = nn;
        posn = nn;
        ++len;
      }
      else{//if posn is end of the list
        Node *nn = new Node{x, nullptr};
        Node *temp = theList;
        for (int i=1;i<len;++i){
          temp = temp->next;
        }
        temp->next = nn;
        posn = nn;
        ++len;
      }
    }
    return posn;
  }

  class const_iterator {
    Node *p;
    const_iterator(Node *p): p{p} {}
   public:
    bool operator!=(const const_iterator &other) const { return p != other.p; }
    const int &operator*() const { return p->data; }
    const_iterator &operator++() {
      p = p->next;
      return *this;
    }
    friend class list;
  };

  const_iterator begin() const { return const_iterator{theList}; }
  const_iterator end() const { return const_iterator{nullptr}; }

  const_iterator cbegin() const { return const_iterator{theList}; }
  const_iterator cend()  const { return const_iterator{nullptr}; }


 private:
  int &get(size_t i) const;  // private helper
};

std::ostream &operator<<(std::ostream &out, const list &l);
}
#endif
