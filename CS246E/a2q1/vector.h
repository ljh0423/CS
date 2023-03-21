#ifndef VECTOR_H
#define VECTOR_H
#include <ostream>
#include <cstddef>
namespace cs246e {
class vector {
  int *theVector;
  size_t n, cap;
 public:
  vector();
  size_t size() const { return n; }
  const int &operator[](size_t i) const { return theVector[i]; }
  int &operator[](size_t i) { return theVector[i]; }
  void push_back(int n);
  void pop_back();
  ~vector();
  typedef int *iterator;
  typedef const int *const_iterator;

  iterator begin() { return theVector; }
  iterator end() { return theVector + n; }

  const_iterator begin() const { return theVector; }
  const_iterator end() const { return theVector + n; }

  class reverse_iterator {
    int *ri, *ve;
   public:
    reverse_iterator (int *ri,int *ve):ri{ri},ve{ve}{} //constructor
    bool operator!=(const reverse_iterator &other) const {if(ri<ve) return 0; else return ri!=other.ri;} //if ri is out of range of vector return false, else do !=
    reverse_iterator &operator++();
    int& operator*() {return *ri;}
    friend class vector;
  };

  class const_reverse_iterator {
    int *ri, *ve;
   public:
    const_reverse_iterator (int *ri,int *ve):ri{ri},ve{ve}{} //constructor
    bool operator!=(const const_reverse_iterator &other) const {if(ri<ve) return 0; else return ri!=other.ri;}
    const_reverse_iterator &operator++();
    const int& operator*() const {return *ri;}
    friend class vector;
  };

  reverse_iterator rbegin() { return reverse_iterator {theVector+n-1, theVector}; }
  reverse_iterator rend() { return reverse_iterator {nullptr, theVector}; }

  const_reverse_iterator rbegin() const { return const_reverse_iterator {theVector+n-1, theVector}; }
  const_reverse_iterator rend() const { return const_reverse_iterator {nullptr, theVector}; }

 private:
  void increaseCap();
};

std::ostream &operator<<(std::ostream &out, const vector &v);
}
#endif
