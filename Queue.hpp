#ifndef Queue_hpp
#define Queue_hpp

#ifndef ARDUINO
#include <cstddef>
#include <cassert>
#else
#include <assert.h>
#endif

template<typename T, size_t Cap>
class Queue {
public:
  Queue();

  T peek() const;
  bool full() const;
  bool empty() const;

  void enqueue(const T& value);
  T dequeue();

private:
  T arr[Cap + 1];
  // end points to the next index after the end of the queue contents. The
  // queue is full when begin is next to end, and empty when begin equals end.
  int begin, end;
};

template<typename T, size_t Cap>
Queue<T, Cap>::Queue()
  : begin(0), end(0) {}

template<typename T, size_t Cap>
T Queue<T, Cap>::peek() const {
  assert(!empty());
  return arr[begin];
}

template<typename T, size_t Cap>
bool Queue<T, Cap>::full() const {
  return (begin + Cap - end) % Cap == 1;
}

template<typename T, size_t Cap>
bool Queue<T, Cap>::empty() const {
  return begin == end;
}

template<typename T, size_t Cap>
void Queue<T, Cap>::enqueue(const T& value) {
  assert(!full());
  arr[end] = value;
  end = (end + 1) % Cap;
}

template<typename T, size_t Cap>
T Queue<T, Cap>::dequeue() {
  assert(!empty());
  T value = arr[begin];
  begin = (begin + 1) % Cap;
  return value;
}

#endif
