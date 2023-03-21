#include <cstddef>
#include <ostream>
#include "vector.h"
namespace CS246E {
const size_t startSize = 1;

vector::vector(): theVector{new int[startSize]}, n{0}, cap{1} {}

void vector::push_back(int x) {
  increaseCap();
  theVector[n++] = x;
}

void vector::pop_back() { if (n > 0) --n; }

vector::~vector() { delete [] theVector; }

void vector::increaseCap() {
  if (n == cap) {
    int *newVec = new int[2 * cap];
    for (size_t i = 0; i < cap; ++i) newVec[i] = theVector[i];
    delete [] theVector;
    theVector = newVec;
    cap *= 2;
  }
}

std::ostream &operator<<(std::ostream &out, const vector &v) {
  for (auto &n : v) out << n << ' ';
  return out;
}
}
