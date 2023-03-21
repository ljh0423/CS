/*
This code construct the deque class using an inner struct ar containing an array of 10 T objects.
I have implemented the default constructor, an initializer list constructor, the big 5 (or 4), and the following methods:
operator[] at begin end empty size
push_front pop_front push_back pop_back
The Deque uses a structure of array of pointers to ar.
*/
#ifndef DEQUE_H
#define DEQUE_H
#include <ostream>
#include <cstddef>
#include <utility>
#include <initializer_list>
namespace cs246e {
template <typename T> class deque {
  struct ar {
    T* t;
    size_t n,f;
    ar(): t{nullptr}, n{0}, f{0} {}
    ar(T* t, size_t n, size_t f): t{t}, n{n}, f{f} {}
    ar(const ar &other): t{other.t!=nullptr ? static_cast <T*> (operator new (10*sizeof(T))) : nullptr}, n{other.n}, f{other.f} {
      if (t!=nullptr){
        if (f) {
        for (size_t i=10-n;i<10;++i){
          new (t + i) T(other.t[i]);
        }
      }
      else {
        for (size_t i=0;i<n;++i){
          new (t + i) T(other.t[i]);
        }
      }
      }
    }
    ar(ar &&other): t{other.t}, n{other.n}, f{other.f} {other.t = nullptr;other.n=0;other.f=0;}
    ar &operator=(ar other){
      std::swap(t, other.t);
      std::swap(n, other.n);
      std::swap(f, other.f);
      return *this;
    }
    void insertf(T x){
      if (n==0) {t=static_cast <T*> (operator new (10*sizeof(T)));}
      new (t+(n++)) T(x);
    }
    void insertb(T x){
      if (n==0) {t=static_cast <T*> (operator new (10*sizeof(T)));}
      new (t+(9-n)) T(x);
      ++n;
      f=10-n;
    }
    void decreasef(){
      t[9-n].~T();
      --n;
      f++;
    }
    void decreaseb(){
      t[n-1].~T();
      n--;
    }
    void clear() {
      if (f) { for (size_t i=10-n;i<10;++i) {t[i].~T();} }
      else while (n) { t[--n].~T(); }
    }
    ~ar() { clear(); operator delete(t); }
  };

  ar **theDeque;
  size_t n, an, fcap, bcap;
 public:
  deque(): theDeque{nullptr}, n{0}, an{0}, fcap{0}, bcap{0} {}
  deque(std::initializer_list <T> init):
    theDeque{new ar*[init.size()%10 ? init.size()/10+1 : init.size()/10]}, n{init.size()}, an{init.size()/10+1}, fcap{0}, bcap{init.size()%10 ? 10-(init.size()%10) : 0} {
      size_t a=0,b=0;
      for (auto &i: init) {
        if(a==10) { b++; a=0; }
        if (a==0) {
          ar *temp = new ar;
          theDeque[b] = temp;
        }
        (*theDeque[b]).insertf(i);
        a++;
      }
    }
  size_t size() const { return n; }
  deque(const deque &other): theDeque{other.theDeque!=nullptr ? new ar*[other.an] : nullptr}, n{other.n}, an{other.an}, fcap{other.fcap}, bcap{other.bcap} {
    if (theDeque!=nullptr){
      for (size_t a=0;a<other.an;++a){
        ar *temp = new ar;
        *temp = *other.theDeque[a];
        theDeque[a] = temp;
      }
    }
  }
  deque(deque &&other): theDeque{other.theDeque}, n{other.n}, an{other.an}, fcap{other.fcap}, bcap{other.bcap} {
    other.theDeque = nullptr;
    other.n = 0;
    other.an = 0;
    other.fcap = 0;
    other.bcap = 0;
  }
  deque &operator=(deque other) {
    std::swap(theDeque, other.theDeque);
    std::swap(n, other.n);
    std::swap(an, other.an);
    std::swap(fcap, other.fcap);
    std::swap(bcap, other.bcap);
    return *this;
  }
  ~deque() {
    for (size_t a=0;a<an;++a){
      delete theDeque[a];
    }
    delete [] theDeque;
  }
  const T &operator[](size_t i) const { 
    ++i;
    size_t a=0;
    while(i > theDeque[a]->n) { i=i-theDeque[a]->n; ++a;}
    if (theDeque[a]->f) {return theDeque[a]->t[10-theDeque[a]->n+i-1];}
    else return theDeque[a]->t[i-1]; }
  T &operator[](size_t i) { 
    ++i;
    size_t a=0;
    while(i > theDeque[a]->n) { i=i-theDeque[a]->n; ++a;}
    if (theDeque[a]->f) {return theDeque[a]->t[10-theDeque[a]->n+i-1];}
    else { return theDeque[a]->t[i-1];} }
  const T &at(size_t i) const {
    ++i;
    if(i > n) throw "Exception";
    size_t a=0;
    while(i > theDeque[a]->n) { i=i-theDeque[a]->n; ++a;}
    if (theDeque[a]->f) {return theDeque[a]->t[10-theDeque[a]->n+i-1];}
    else return theDeque[a]->t[i-1]; }
  T &at(size_t i) {
    ++i;
    if(i > n) throw "Exception";
    size_t a=0;
    while(i > theDeque[a]->n) { i=i-theDeque[a]->n; ++a;}
    if (theDeque[a]->f) {return theDeque[a]->t[10-theDeque[a]->n+i-1];}
    else return theDeque[a]->t[i-1]; }
  
  class iterator {
    T* p;
    deque &d;
    size_t a;
    iterator(T *p, deque &d, size_t a): p{p}, d{d}, a{a} {}
   public:
    bool operator!=(const iterator &other) const { return p != other.p; }
    T &operator*() const { return *p; }
    iterator &operator++() {
      size_t b=0;
      if(d.fcap && a==0) {
        b=10-d.theDeque[0]->n;
        for (;b<10;++b){
          if(d.theDeque[a]->t+b==p) {
            if (a==d.an-1 && b==9) { p=nullptr; return *this; }
            else if (b==9) { a++; p = d.theDeque[a]->t; return *this; }
            else { p++; return *this; }
          }
        }
      }
      else {
        for (;b<d.theDeque[a]->n;++b){
        if (d.theDeque[a]->t+b==p) {
          if (a==d.an-1 && b==d.theDeque[a]->n-1) { p=nullptr; return *this; }
          else if (b==9) { a++; p = d.theDeque[a+1]->t; return *this; }
          else { p++; return *this; }
          }
        }
      }
      }
    friend class deque;
  };
  class const_iterator {
    T* p;
    deque &d;
    size_t a;
    const_iterator(T *p, deque &d, size_t a): p{p}, d{d}, a{a} {}
   public:
    bool operator!=(const const_iterator &other) const { return p != other.p; }
    const T &operator*() const { return *p; }
    const_iterator &operator++() {
      size_t b=0;
      if(d.fcap && a==0) {
        b=10-d.theDeque[0]->n;
        for (;b<10;++b){
          if(d.theDeque[a]->t+b==p) {
            if (a==d.an-1 && b==9) { p=nullptr; return *this; }
            else if (b==9) { a++; p = d.theDeque[a]->t; return *this; }
            else { p++; return *this; }
          }
        }
      }
      else {
        for (;b<d.theDeque[a]->n;++b){
        if (d.theDeque[a]->t+b==p) {
          if (a==d.an-1 && b==d.theDeque[a]->n-1) { p=nullptr; return *this; }
          else if (b==9) { a++; p = d.theDeque[a+1]->t; return *this; }
          else { p++; return *this; }
          }
        }
      }
      }
    friend class deque;
  };

  iterator begin() { 
    if(theDeque==nullptr) return iterator{nullptr, *this, 0};
    else if(fcap) return iterator{theDeque[0]->t+(10-theDeque[0]->n), *this, 0};
    else return iterator{theDeque[0]->t, *this, 0};
  }
  iterator end() { return iterator{nullptr, *this, an}; }

  const_iterator begin() const { 
    if(theDeque==nullptr) return const_iterator{nullptr, *this, 0};
    else if(fcap) return const_iterator{theDeque[0]->t+(10-theDeque[0]->n), *this, 0};
    else return const_iterator{theDeque[0]->t, *this, 0};
  }
  const_iterator end() const { return const_iterator{nullptr, *this, an}; }
  
  bool empty() {return n==0;}

  void push_front(T it) {
    if (fcap==0){
      ar** tmp = new ar*[an + 1];
      ar* tp = new ar;
      (*tp).insertb(it);
      tmp[0] = tp;
      for (size_t a=0;a<an;++a){
        ar *temp = new ar;
        *temp = *theDeque[a];
        tmp[a+1] = temp;
      }
      (*this).~deque();
      theDeque = tmp;
      ++n;
      ++an;
      fcap = 9;
    }
    else {
      (*theDeque[0]).insertb(it);
      ++n;
      --fcap;
    }
  }
  void push_back(T it) {
    if (bcap==0){
      ar** tmp = new ar*[an + 1];
      ar *tp = new ar;
      (*tp).insertf(it);
      for (size_t a=0;a<an;++a){
        ar *temp = new ar;
        *temp = *theDeque[a];
        tmp[a] = temp;
      }
      tmp[an] = tp;
      (*this).~deque();
      theDeque = tmp;
      ++n;
      ++an;
      bcap = 9;
    }
    else {
      (*theDeque[an-1]).insertf(it);
      ++n;
      --bcap;
    }
  }
  void pop_front(){
    if (n<=1){
      (*this).~deque();
      theDeque = nullptr;
      n=0;
      an=0;
      fcap=0;
      bcap=0;
    }
    else if(fcap==9){
      ar** tmp = new ar*[an-1];
      for (size_t a=1;a<an;++a){
        ar *temp = new ar;
        *temp = *theDeque[a];
        tmp[a-1] = temp;
      }
      (*this).~deque();
      theDeque = tmp;
      --n;
      --an;
      fcap=0;
    }
    else if (fcap || theDeque[0]->n==10) {
      (*theDeque[0]).decreasef();
      --n;
      ++fcap;
    }
    else {
      theDeque[0]->t[0].~T();
      for (size_t i=0;i<theDeque[0]->n-1;++i){
        theDeque[0]->t[i] = theDeque[0]->t[i+1];
      }
      theDeque[0]->t[theDeque[0]->n-1].~T();
      theDeque[0]->n--;
      --n;
      ++bcap;
    }
  }
  void pop_back(){
    if (n<=1){
      (*this).~deque();
      theDeque = nullptr;
      n=0;
      an=0;
      fcap=0;
      bcap=0;
    }
    else if(bcap==9){
      ar** tmp = new ar*[an-1];
      for (size_t a=0;a<an-1;++a){
        ar *temp = new ar;
        *temp = *theDeque[a];
        tmp[a] = temp;
      }
      (*this).~deque();
      theDeque = tmp;
      --n;
      --an;
      bcap=0;
    }
    else if (bcap || theDeque[an-1]->n==10) {
      (*theDeque[an-1]).decreaseb();
      --n;
      ++bcap;
    }
    else {
      theDeque[0]->t[9].~T();
      for (size_t i=9;i>10-theDeque[0]->n;--i){
        theDeque[0]->t[i] = theDeque[0]->t[i-1];
      }
      theDeque[0]->t[10-theDeque[0]->n].~T();
      theDeque[0]->n--;
      theDeque[0]->f++;
      --n;
      ++fcap;
    }
  }

};

}
#endif
