#include <iostream>

// Dude this got big fast
// long shlong struct
template <typename T> struct DynamicList {
  T data;
  DynamicList<T> *next = nullptr;
  bool initialized = false; // This allows for creation of the List without //
                            // declaring first data, might be a better way tho
  void push(T element) {
    if (initialized && next == nullptr) {
      DynamicList<T> *newNode = new DynamicList<T>;
      newNode->data = element;
      newNode->next = next;
      newNode->initialized = true;
      next = newNode;
    } else if (!initialized) {
      this->data = element;
      this->initialized = true;
    } else {
      next->push(element);
    }
  };

  void pad() {
    if (next == nullptr) {
      DynamicList<T> *newNode = new DynamicList<T>;
      newNode->next = next;
      newNode->initialized = false;
      next = newNode;
    } else {
      next->pad();
    }
  }

  void insert(T element, unsigned index) {
    unsigned len = this->len();
    if (index >= len) {
      for (unsigned i = len; i < index; i++) {
        this->pad();
      }
      this->push(element);
    } else {
      DynamicList<T> *chosen = this->get_pointer(index);
      chosen->data = element;
      chosen->initialized = true;
    }
  }

  // Might be useful at some point
  unsigned len() {
    if (initialized && next == nullptr) {
      return 1;
    } else if (next != nullptr) {
      return next->len() + 1;
    } else {
      return 0;
    }
  }

  // Return a pointer to data of the specified index
  // i don't know how to do the nice [index] thingy
  // so this has to do
  DynamicList<T> *get_pointer(unsigned index) {
    if (index == 0) {
      return this;
    } else {
      return next->get_pointer(index - 1);
    }
  }

  T get(unsigned index) {
    if (index == 0) {
      return data;
    } else {
      return next->get(index - 1);
    }
  }

  // It was needlsly complex, it's separate addresses moving everything is just
  // plain dumb, so we just delete the one we want and replace it with the next
  // element, which hold the next one already
  void remove(unsigned index) {
    if (index == 0) {
      if (next != nullptr) {
        DynamicList<T> *temp = next;
        data = temp->data;
        initialized = temp->initialized;
        next = temp->next;
        delete temp;
      } else {
        delete this;
      }

    } else {
      next->remove(index - 1);
    }
  }

  // Using bubble sort for simplicity, lambdas didn't work with the function<>
  // shit but work with a pointer, so fuck you geeksforgeeks, you and ur shitty
  // guide
  void sort(bool (*cmp)(T, T)) {
    int len = this->len();
    bool swapped;

    for (int i = 0; i < len - 1; i++) {
      swapped = false;
      for (int j = 0; j < len - i - 1; j++) {
        DynamicList<T> *test = this->get_pointer(j);
        DynamicList<T> *next = test->next;
        if (cmp(test->data, next->data)) {
          T temp = test->data;
          test->data = next->data;
          next->data = temp;
          swapped = true;
        }
      }
      if (!swapped)
        break;
    }
  }
};

struct Edge {
  unsigned vertex1;
  unsigned vertex2;
  unsigned weight;
  // Overloading breaks passing the function and i have no idea how to pass an
  // overloaded one
  static bool cmp(Edge edge1, Edge edge2) {
    return edge1.weight > edge2.weight;
  };
};

struct Graph {
  DynamicList<Edge> edges;
  DynamicList<unsigned> vertices;
  DynamicList<DynamicList<bool>> neighbors_matrix;

  void add(unsigned vertex1, unsigned vertex2, unsigned weight) {
    edges.push({vertex1, vertex2, weight});

    bool vertex1_in_list = false;
    bool vertex2_in_list = false;
    for (int i = 0; i < vertices.len(); i++) {
      if (vertices.get(i) == vertex1) {
        vertex1_in_list = true;
      }
      if (vertices.get(i) == vertex2) {
        vertex2_in_list = true;
      }
    }
    if (!vertex1_in_list) {
      vertices.push(vertex1);
    }
    if (!vertex2_in_list) {
      vertices.push(vertex2);
    }

    if (neighbors_matrix.len() <= vertex1) {
      DynamicList<bool> temp;
      temp.insert(true, vertex2);
      neighbors_matrix.insert(temp, vertex1);
    } else {
      neighbors_matrix.get_pointer(vertex1)->data.insert(true, vertex2);
    }
    // It's a rectangle so it should work no matter the vertex order
    if (neighbors_matrix.len() <= vertex2) {
      DynamicList<bool> temp;
      temp.insert(true, vertex1);
      neighbors_matrix.insert(temp, vertex2);
    } else {
      neighbors_matrix.get_pointer(vertex2)->data.insert(true, vertex1);
    }
  }

  bool are_connected(unsigned vertex1, unsigned vertex2) {
    return neighbors_matrix.get(vertex1).get(vertex2);
  }
};

Graph rebuild(Graph graph, DynamicList<unsigned> vertices) {
  Graph new_graph;
  for (int i = 0; i < graph.edges.len(); i++) {
    Edge temp = graph.edges.get(i);
    for (int j = 0; j < vertices.len(); j++) {
      if (vertices.get(j) == temp.vertex1 || vertices.get(j) == temp.vertex2) {
        new_graph.add(temp.vertex1, temp.vertex2, temp.weight);
        break;
      }
    }
  }
  return new_graph;
}

// Straight i keep forgetting my logic as i write, subconcious solving idk
void traverse(Graph graph, unsigned vertex, DynamicList<bool> &visited) {
  visited.insert(true, vertex);
  for (int v = 0; v < graph.vertices.len(); v++) {
    if (v > visited.len()) {
      visited.insert(false, v);
    }
    // Yeah i need the vertex id instead of index so we grab it or something
    unsigned u = graph.vertices.get(vertex);
    if (graph.are_connected(u, v) &&
        (visited.get(v) == false ||
         visited.get_pointer(v)->initialized == false)) {
      traverse(graph, v, visited);
    }
  }
}

// I grabbed the algorithm from the internet and modded to fit
void split_separate(Graph graph, DynamicList<Graph> &push_here) {
  DynamicList<bool> visited;
  DynamicList<unsigned> connected;
  DynamicList<unsigned> leftovers; // imma eat them

  for (int u = 0; u < graph.vertices.len(); u++) {
    for (int i = 0; i < graph.vertices.len(); i++) {
      visited.insert(false, i);
      traverse(graph, u, visited);
    }
  }
  for (int i = 0; i < visited.len(); i++) {
    if (visited.get(i) == true) {
      connected.push(graph.vertices.get(i));
    } else {
      leftovers.push(graph.vertices.get(i));
    }
  }
  push_here.push(rebuild(graph, connected));
  if (leftovers.len() != 0) {
    Graph what_is_this_city = rebuild(graph, leftovers);
    split_separate(what_is_this_city, push_here);
  }
}

int main() {
  int len;
  Graph list;
  std::cout << "Array length: ";
  std::cin >> len;
  for (int i = 0; i < len; i++) {
    unsigned vertex1, vertex2, weight;

    std::cout << "Edge in format: vertex1 vertex2 weight" << i + 1 << ": ";
    std::cin >> vertex1 >> vertex2 >> weight;
    list.add(vertex1, vertex2, weight);
  }

  list.edges.sort(&Edge::cmp);
  len = list.edges.len();
  std::cout << "Measured length: " << len << std::endl;
  std::cout << "Sorted array: " << std::endl;
  for (int i = 0; i < len; i++) {
    std::cout << list.edges.get(i).vertex1 << " " << list.edges.get(i).vertex2
              << " " << list.edges.get(i).weight << std::endl;
  }
  return 0;
}
