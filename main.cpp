#include <iostream>
// If this is considered STL imma loose my fucking mind
#include <functional>

template <typename T> struct DynamicList {
  T data;
  DynamicList<T> *next;
  bool initialized = false; // This allows for creation of the List without
                            // declaring first data, might be a better way tho
  void push(T element) {
    DynamicList<T> *newNode = new DynamicList<T>;
    newNode->data = element;
    newNode->next = nullptr;
    newNode->initialized = true;
    if (initialized && next == nullptr) {
      next = newNode;
    } else if (!initialized) {
      data = element;
      initialized = true;
    } else {
      next->push(element);
    }
  };

  // Might be useful at some point
  unsigned len() {
    if (initialized && next == nullptr) {
      return 1;
    } else if (initialized) {
      return next->len() + 1;
    } else {
      return 0;
    }
  }

  // Return a pointer to data of the specified index
  // i don't know how to do the nice [index] thingy
  // so this has to do
  T *get(unsigned index) {
    if (index == 0) {
      return this->data;
    } else {
      return next->get(index - 1);
    }
  }

  // I have no idea if this works, pointers into pointers with pointers get's
  // complicated fast
  void remove(unsigned index) {
    if (index == 0) {
      T hold;
      for (int i = len(); i > 0; --i) {
        hold = get(i - 1);
        get(i - 1) = get(i);
        remove(i);
      }
    } else {
      next->remove(index - 1);
    }
  }

  // Using bubble sort for simplicity, if the <functional> is an STL (i don't
  // think it is tho) then we can pass a pointer to a function instead, but then
  // we don't have lambdas when we want oneline functions
  // also have a tutorial
  // https://www.geeksforgeeks.org/passing-a-function-as-a-parameter-in-cpp/
  void sort(std::function<bool(T *a, T *b)> cmp) {
    int len = this->len();
    bool swapped;

    for (int i = 0; i < len - 1; i++) {
      swapped = false;
      for (int j = 0; j < len - i - 1; j++) {
        if (cmp(data, next->data)) {
          T temp = data;
          data = next->data;
          next->data = temp;
          swapped = true;
        }
      }
    }
  }
};

struct Edge {
  unsigned vertex1;
  unsigned vertex2;
  unsigned weight;
  // Overloading the function to allow calling on itself or idnependently
  bool cmp(Edge *edge) { return weight > edge->weight; };
  bool cmp(Edge *edge1, Edge *edge2) { return edge1->weight > edge2->weight; };
};

int main() {
  std::cout << "Hello World!" << std::endl;
  return 0;
}
