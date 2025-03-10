#include <iostream>

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

  // TODO: Actually implement sorting, with passing a compare function
  //  i have to look into how to do this. Also we need to pick a sorting
  //  algorithm bubble sort would be easy, but we might want something fancier
  //  and faster
  void sort() {}
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
