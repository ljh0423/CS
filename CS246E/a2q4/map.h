/*
This code construct the map class using an inner struct node containing pointers to the parent, left child, and right child node, with a pair stored as data..
I have implemented the default constructor, an initializer list constructor, the big 5 (or 4), and the following methods:
operator[] at begin end empty size erase(const Key &k) clear count
The Map contains a pointer to the root node of the binary tree, which allow these methods to be performed.
*/
#ifndef MAP_H
#define MAP_H
#include <ostream>
#include <utility>
#include <iostream>
#include <initializer_list>
#include <cstddef>
namespace cs246e {
template <class K, class T> class map {
  struct node {
    node *par;
    node *l;
    node *r;
    std::pair<const K,T> *p;
    node(): par{nullptr}, l{nullptr}, r{nullptr}, p{nullptr} {}
    node(node *par, node *l, node *r, std::pair<K,T> *p): par{par}, l{l}, r{r}, p{p} {}
    node(const node &other): par{nullptr}, l{other.l!=nullptr? new node{*other.l,this} : nullptr}, r{other.r!=nullptr? new node{*other.r,this} : nullptr}, p{other.p!=nullptr ? static_cast <std::pair<const K, T>*> (operator new (sizeof(std::pair<const K, T>))) : nullptr} {
      if(p!=nullptr) {
        new (p) std::pair<const K, T>(*other.p);
      }
    }
    node(const node &other,node* parent): par{parent}, l{other.l!=nullptr? new node{*other.l,this} : nullptr}, r{other.r!=nullptr? new node{*other.r,this} : nullptr}, p{other.p!=nullptr ? static_cast <std::pair<const K, T>*> (operator new (sizeof(std::pair<const K, T>))) : nullptr} {
      if(p!=nullptr) {
        new (p) std::pair<const K, T>(*other.p);
      }
    }
    node(node &&other): par{other.par}, l{other.l}, r{other.r}, p{other.p} {
      other.par=nullptr;
      other.l=nullptr;
      other.r=nullptr;
      other.p=nullptr;
    }
    node &operator=(node other){
      std::swap(par,other.par);
      std::swap(l,other.l);
      std::swap(r,other.r);
      std::swap(p,other.p);
      return *this;
    }
    std::pair<const K, T> &operator*() { return *p; }
    node* leftmost(node* n){
      node *tp=n;
      while(tp!=nullptr){ if(tp->l==nullptr) break; tp = tp->l;}
      return tp;
    }
    void clear() {
      operator delete(p);
    }
    ~node() {
      clear();
    }
  };
  node* theMap;
  size_t n;
 public:
  map(): theMap{nullptr}, n{0} {}
  map(std::initializer_list <std::pair<const K,T>> init): theMap{ nullptr }, n{0} {
    for (auto &i: init){
      std::pair<const K,T> *tp = new std::pair<const K,T> {i.first,i.second};
      this->insert(tp);
    }
  }
  map(const map &other): theMap{other.theMap!=nullptr ? new node{*other.theMap} : nullptr}, n{other.n} {}
  map(map &&other): theMap{other.theMap}, n{other.n} {
    other.theMap = nullptr;
    other.n = 0;
  }
  map &operator=(map other) {
    std::swap(theMap, other.theMap);
    std::swap(n, other.n);
    return *this;
  }
  ~map() {
    if(theMap!=nullptr){
      node *f = theMap;
      if(f->l!=nullptr) { theMap = f->l; this->~map(); }
      if(f->r!=nullptr) { theMap = f->r; this->~map(); }
      delete f;
    }
  }
  size_t size() const { return n; }
  bool empty() {return n==0;}

  T &operator[](const K &key) {
    node *f = theMap;
    while(f!=nullptr){
      if(f->p->first == key) {return f->p->second;}
      else if(f->p->first < key) { if(f->r==nullptr) break; f = f->r; }
      else if(key < f->p->first) { if(f->l==nullptr) break; f = f->l; }
    }
    std::pair<const K,T> *tp = new std::pair<const K,T> {key,T()};
    this->insert(tp);
    return tp->second;
  }
  T &operator[](K &&key) {
    node *f = theMap;
    while(f!=nullptr){
      if(f->p->first == key) {return f->p->second;}
      else if(f->p->first < key) { if(f->r==nullptr) break; f = f->r; }
      else if(key < f->p->first) { if(f->l==nullptr) break; f = f->l; }
    }
    std::pair<const K,T> *tp = new std::pair<const K,T> {key,T()};
    this->insert(tp);
    return tp->second;
  }
  T &at(const K key) {
    node *f = theMap;
    while(f!=nullptr){
      if(f->p->first == key) {return f->p->second;}
      else if(f->p->first < key) { if(f->r==nullptr) break; f = f->r; }
      else if(key < f->p->first) { if(f->l==nullptr) break; f = f->l; }
    }
    throw "out of range";
  }
  const T &at(const K key) const {
    node *f = theMap;
    while(f!=nullptr){
      if(f->p->first == key) {return f->p->second;}
      else if(f->p->first < key) { if(f->r==nullptr) break; f = f->r; }
      else if(key < f->p->first) { if(f->l==nullptr) break; f = f->l; }
    }
    throw "out of range";
  }
  class iterator {
    node *refp;
    iterator(node *refp): refp{refp} {}
   public:
    bool operator!=(const iterator &other) const { return refp != other.refp; }
    std::pair<const K,T> &operator*() const { return *(*refp); }
    iterator &operator++() {
      if(refp==nullptr) {std::cerr<<"error";}
      node *cur = refp;
      while(cur!=nullptr){
        if (refp->r!=nullptr){
          refp = (*refp).leftmost(refp->r);
          return *this;
        }
        else {
          if(cur->par==nullptr) { refp = nullptr; return *this;}
          else if(cur->par->l==cur) { refp = cur->par; return *this; }
          else if(cur->par->r==cur) { cur = cur->par; }
        }
      }
    }
    friend class map;
  };
  class const_iterator {
    node *refp;
    const_iterator(node *refp): refp{refp} {}
   public:
    bool operator!=(const const_iterator &other) const { return refp != other.refp; }
    const std::pair<const K,T> &operator*() const { return *(*refp); }
    const_iterator &operator++() {
      if(refp==nullptr) {std::cerr<<"error";}
      node *cur = refp;
      while(cur!=nullptr){
        if (refp->r!=nullptr){
          refp = (*refp).leftmost(refp->r);
          return *this;
        }
        else {
          if(cur->par==nullptr) { refp = nullptr; return *this;}
          else if(cur->par->l==cur) { refp = cur->par; return *this; }
          else if(cur->par->r==cur) { cur = cur->par; }
        }
      }
    }
    friend class map;
  };
  iterator begin() { 
    if(theMap==nullptr) return iterator{nullptr};
    else return iterator{(*theMap).leftmost(theMap)};
  }
  iterator end() { return iterator{nullptr}; }
  const_iterator begin() const { 
    if(theMap==nullptr) return const_iterator{nullptr};
    else return const_iterator{(*theMap).leftmost(theMap)};
  }
  const_iterator end() const { return const_iterator{nullptr}; }

  size_t erase(const K &k){
    node *f = theMap;
    while(f!=nullptr){
      if(f->p->first == k) {
        if (f->l==nullptr && f->r==nullptr){
          if(f->par!=nullptr){
            if(f->par->l==f) f->par->l = nullptr;
            else if(f->par->r==f) f->par->r = nullptr;
          }
          else theMap = nullptr;
          delete f;
        }
        else if (f->l!=nullptr && f->r!=nullptr){
          node* s = (*f).leftmost(f->r);
          std::swap(f->p,s->p);
          node* f = theMap;
          theMap = f->r;
          (*this).erase(s->p->first);
          theMap = f;
        }
        else if (f->l!=nullptr){
          node* tp = f->l;
          std::swap(f->p,tp->p);
          f->r = tp->r;
          if(f->r!=nullptr) f->r->par = f;
          f->l = tp->l;
          if(f->l!=nullptr) f->l->par = f;
          delete tp;
        }
        else if (f->r!=nullptr){
          node* tp = f->r;
          std::swap(f->p,tp->p);
          f->r = tp->r;
          if(f->r!=nullptr) f->r->par = f;
          f->l = tp->l;
          if(f->l!=nullptr) f->l->par = f;
          delete tp;
        }
        --n;
        return 1;
      }
      else if(f->p->first < k) { if(f->r==nullptr) break; f = f->r; }
      else if(k < f->p->first) { if(f->l==nullptr) break; f = f->l; }
    }
    return 0;
  }
  void clear(){
    (*this).~map();
    theMap = nullptr;
  }

  size_t count(const K k){
    node *f = theMap;
    while(f!=nullptr){
      if(f->p->first == k) {return 1;}
      else if(f->p->first < k) { if(f->r==nullptr) break; f = f->r; }
      else if(k < f->p->first) { if(f->l==nullptr) break; f = f->l; }
    }
    return 0;
  }

 private:
  void insert(std::pair<const K,T> *e){
    node *newn = new node;
    newn->p = e;
    if(theMap==nullptr){
      theMap = newn;
    }
    else {
      node *f = theMap;
      while(f!=nullptr){
        if(f->p->first < e->first) {
          if(f->r==nullptr){
            newn->par = f;
            f->r = newn;
            break;
          }
          f = f->r;
        }
        else if(e->first < f->p->first) {
          if(f->l==nullptr){
            newn->par = f;
            f->l = newn;
            break;
          }
          f = f->l;
        }
      }
    }
    ++n;
  }
  map(node* theMap): theMap{theMap}, n{1} {}
};

}
#endif
