#include <iostream>

// TODO: 1. fuckin test this bullshit
//       2. rewrite the input portion to better reflect what this code does
//       3. implement the Kruskal Algorithm (like the task wants)
//       4. output the result

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
    } else if (!initialized && next == nullptr) {
      this->data = element;
      this->initialized = true;
    } else {
      next->push(element);
    }
  };

  void pad() {
    if (next == nullptr) {
      DynamicList<T> *newNode = new DynamicList<T>;
      newNode->next = nullptr;
      newNode->initialized = false;
      next = newNode;
    } else {
      next->pad();
    }
  }

  void insert(T element, unsigned index) {
    unsigned len = this->len();
    if (index >= len) {
      for (unsigned i = len; i <= index; i++) {
        this->pad();
      }
      this->get_pointer(index)->data = element;
      this->get_pointer(index)->initialized = true;
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

  bool contains(T element) {
    if (initialized && data == element) {
      return true;
    } else if (next != nullptr) {
      return next->contains(element);
    } else {
      return false;
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
  DynamicList<DynamicList<Edge>>
      body;                // Used for dealing with if it's neighboring
  DynamicList<Edge> edges; // Holds all the given edges, used for sorting

  void add(Edge edge) {
    edges.push(edge); // For sorting

    std::cout << "Pre add len: " << body.len() << std::endl;
    if (body.len() <= edge.vertex1) {
      body.insert({edge, nullptr, true}, edge.vertex1);
    } else {
      std::cout << "Pushing" << std::endl;
      body.get_pointer(edge.vertex1)->data.push(edge);
    }

    if (body.len() <= edge.vertex2) {
      body.insert({edge, nullptr, true}, edge.vertex2);
    } else {
      body.get_pointer(edge.vertex2)->data.push(edge);
    }
  }

  bool are_connected(unsigned vertex1, unsigned vertex2) {
    DynamicList<Edge> list = body.get(vertex1);
    for (unsigned i = 0; i < list.len(); i++) {
      if (list.get(i).vertex2 == vertex2) {
        return true;
      }
    }
    return false;
  }
};

void traverse(Graph graph, unsigned vertex, DynamicList<unsigned> &visits) {
  visits.push(vertex);
  std::cout << "Visiting " << vertex << std::endl;
  DynamicList<Edge> edgeList = graph.body.get(vertex);
  std::cout << "len: " << graph.body.get(vertex).len() << std::endl;
  for (unsigned i = 0; i < edgeList.len(); i++) {
    Edge edge = edgeList.get(i);
    std::cout << "Can go to: " << edge.vertex1 << " " << edge.vertex2
              << std::endl;
    if (!visits.contains(edge.vertex2)) {
      traverse(graph, edge.vertex2, visits);
    }
    if (!visits.contains(edge.vertex1)) {
      traverse(graph, edge.vertex1, visits);
    }
  }
}

bool wouldFormLoop(Graph graph, Edge edge) {
  if (graph.body.len() <= edge.vertex1 || graph.body.len() <= edge.vertex2) {
    std::cout << "SHORT" << std::endl;
    return false;
  }
  DynamicList<unsigned> visited;
  traverse(graph, edge.vertex1, visited);
  return visited.contains(edge.vertex2);
}

int main() {
  int len;
  Graph graph;
  std::cout << "Amount of edges: ";
  std::cin >> len;
  for (int i = 0; i < len; i++) {
    unsigned vertex1, vertex2, weight;

    std::cout << "Edge " << i + 1 << " in format: vertex1 vertex2 weight:";
    std::cin >> vertex1 >> vertex2 >> weight;
    graph.add({vertex1 - 1, vertex2 - 1, weight});
  }

  graph.edges.sort(&Edge::cmp);
  std::cout << "Length: " << graph.body.len() << std::endl;
  for (unsigned i = 0; i < graph.body.len(); i++) {
    std::cout << "vertex " << i << "neighbors: " << graph.body.get(i).len()
              << std::endl;
  }
  Graph output;
  for (unsigned i = 0; i < graph.edges.len(); i++) {
    std::cout << "Checking " << i << std::endl;
    if (!wouldFormLoop(output, graph.edges.get(i))) {
      output.add(graph.edges.get(i));
      std::cout << "Added edge: "
                << output.edges.get(output.edges.len() - 1).vertex1 << " "
                << output.edges.get(output.edges.len() - 1).vertex2 << " "
                << output.edges.get(output.edges.len() - 1).weight << std::endl;
      std::cout << "After add length: " << output.body.len() << std::endl;
      for (unsigned i = 0; i < output.body.len(); i++) {
        std::cout << "vertex " << i << "neighbors: " << output.body.get(i).len()
                  << std::endl;
      }
    }
  }

  std::cout << "Created new graph with " << output.edges.len() << " edges:\n";
  for (unsigned i = 0; i < output.edges.len(); i++) {
    Edge edge = output.edges.get(i);
    std::cout << edge.vertex1 + 1 << " " << edge.vertex2 + 1 << " "
              << edge.weight << "\n";
  }

  return 0;
}
