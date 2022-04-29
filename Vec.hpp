#ifndef Vec_hpp
#define Vec_hpp

#ifndef ARDUINO
#include <cstddef>
#include <cassert>
#else
#include <assert.h>
#endif

#define MIN(a, b) (a) < (b) ? (a) : (b)

/** 
 * Vec is a class that offers the capabilities of vectors, with the exception of
 * a tight maximum size cap that is imposed by the user on creation. Memory is
 * only statically allocated for compatibility with Arduino. Provides a minimal
 * set of functionality.
 */

template<typename T, size_t Cap>
class Vec {
public:
  Vec();
  Vec(const T (&values)[Cap]);
  Vec(const Vec<T, Cap>& other);
  Vec(size_t count, T value);

  Vec& operator=(const Vec& vec);

  const T& operator[](size_t index) const;
  T& operator[](size_t index);

  size_t size() const;
  size_t capacity() const;
  bool empty() const;
  
  void add(const T& value);
  T& pop();

  static void reverse(Vec<T, Cap>& vec);
  
private:
  T arr[Cap];
  size_t sz;
};

template<typename T>
using Vec4 = Vec<T, 4>;

template<typename T>
using Vec8 = Vec<T, 8>;

template<typename T>
using Vec16 = Vec<T, 16>;

template<typename T>
using Vec32 = Vec<T, 32>;

template<typename T>
using Vec64 = Vec<T, 64>;

template<typename T>
using Vec256 = Vec<T, 256>;

template<typename T>
using Vec512 = Vec<T, 512>;

template<typename T, size_t Cap>
Vec<T, Cap>::Vec()
  : sz(0) {}

template<typename T, size_t Cap>
Vec<T, Cap>::Vec(const T (&values)[Cap])
  : sz(Cap) {
  for (int i = 0; i < Cap; i++)
    arr[i] = values[i];
}

template<typename T, size_t Cap>
Vec<T, Cap>::Vec(const Vec<T, Cap>& other)
  : sz(other.sz) {
  for (int i = 0; i < other.sz; i++)
    arr[i] = other.arr[i];
}

template<typename T, size_t Cap>
Vec<T, Cap>::Vec(size_t count, T value)
  : sz(MIN(count, Cap)) {
  for (int i = 0; i < sz; i++)
    arr[i] = value;
}

template<typename T, size_t Cap>
Vec<T, Cap>& Vec<T, Cap>::operator=(const Vec<T, Cap>& vec)
{
  sz = vec.sz;
  for (int i = 0; i < sz; i++)
    arr[i] = vec.arr[i];
  return *this;
}

template<typename T, size_t Cap>
const T& Vec<T, Cap>::operator[](size_t index) const {
  assert(index < sz);
  return arr[index];
}

template<typename T, size_t Cap>
T& Vec<T, Cap>::operator[](size_t index) {
  assert(index < sz);
  return arr[index];
}

template<typename T, size_t Cap>
size_t Vec<T, Cap>::size() const {
  return sz;
}

template<typename T, size_t Cap>
size_t Vec<T, Cap>::capacity() const {
  return Cap;
}

template<typename T, size_t Cap>
bool Vec<T, Cap>::empty() const {
  return sz == 0;
}

template<typename T, size_t Cap>
void Vec<T, Cap>::add(const T& value) {
  assert(sz < Cap);
  arr[sz++] = value;
}

template<typename T, size_t Cap>
T& Vec<T, Cap>::pop() {
  assert(sz > 0);
  return arr[--sz];
}

template<typename T, size_t Cap>
void Vec<T, Cap>::reverse(Vec<T, Cap> &vec) {
  for (int i = 0; i < vec.sz / 2; i++) {
    T front = vec[i];
    vec[i] = vec[vec.sz - i - 1];
    vec[vec.sz - i - 1] = front;
  }
}

#endif
