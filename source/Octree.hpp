#include <octree_utils.hpp>

#define COLOR int //more facha :v
#define NUM_CHILDS 8
#define BLACK_NODE 0 //no children, full content
#define WHITE_NODE 1 //no children, no content
#define INTERVAL_NODE 2 //has children, no content

template<size_t height>
class OcTree {
private:
  struct Octant {
    Vector3d bound_min, bound_max, middle, child_size, own_size;
    array<shared_ptr<Octant>, NUM_CHILDS> children = { nullptr };
    vector<shared_ptr<Vector3d>> content;
    shared_ptr<Octant> parent;
    COLOR color;

    Octant(Vector3d begin, Vector3d size, shared_ptr<Octant> parent);
    bool contains(Vector3d point);
    bool isLeaf();
  };
  shared_ptr<Octant> root;
  vector<Vector3d> data_set;

  void insert(Vector3d newPoint, shared_ptr<Octant> &current);
  void split(shared_ptr<Octant> &node, size_t current_level);
  void recall(shared_ptr<Octant> &node, COLOR condition = BLACK_NODE);
public:
  OcTree(string path_file);
  ~OcTree();

  void read_OcTree(shared_ptr<Octant> node);
  shared_ptr<Octant> getBegin(){ return root; }
};

template<size_t height>
OcTree<height>::Octant::Octant(Vector3d begin, Vector3d size, shared_ptr<Octant> parent) {
  bound_min = begin;
  bound_max = begin + size;
  own_size = size;
  child_size = size * 0.5;
  middle = bound_min + child_size;
  this->parent = parent;
  color = WHITE_NODE;
}

template<size_t height>
bool OcTree<height>::Octant::contains(Vector3d point) {
  return (bound_min < point) && (point <= bound_max);
}

template<size_t height>
bool OcTree<height>::Octant::isLeaf() {
  return BLACK_NODE == color || color == WHITE_NODE;
}

//###################################################################################

template<size_t height>
OcTree<height>::OcTree(string path_file) {
  try {
    if (height > size_t(100)) {
      throw runtime_error(ERROR_FILL_VALUE);
    }

    ifstream obj3d_file(path_file);
    if (obj3d_file.is_open()) {
      ALERT(">>file " + path_file.substr(0, size_t(path_file.length()) - 4) + " openned");
      pair<Vector3d,Vector3d> bounds = get_Points(obj3d_file, data_set);
      root = make_shared<Octant>(bounds.first, bounds.second - bounds.first, nullptr);
      ALERT("Data Set size: " + to_string(int(data_set.size())));
      DrawableOctant cube_root(root->middle, root->own_size, true);
      d_octants.push_back(cube_root);

      shared_ptr<Octant> current = root;
      split(current, size_t(1));

      for (Vector3d vec : data_set) {
        current = root;
        insert(vec, current);
      }
      ALERT("ALL DATA  SAVED");
    }
    else {
      throw runtime_error(ERROR_FILE_NOT_FOUND);
    }
  }
  catch (const exception &error) {
    ALERT(error.what());
  }
}

template<size_t height>
OcTree<height>::~OcTree() {
  root.reset();
}

template<size_t height>
void OcTree<height>::insert(Vector3d newPoint, shared_ptr<Octant> &current) {
  if (!current->isLeaf()) {
    for (size_t i = size_t(0); i < NUM_CHILDS; ++i) {
      if (current->children[i]->contains(newPoint)) { insert(newPoint, current->children[i]); }
    }
  }
  else {
    shared_ptr<Vector3d> new_point_ptr = make_shared<Vector3d>(newPoint.x, newPoint.y, newPoint.z);
    current->content.push_back(new_point_ptr);
    current->color = BLACK_NODE;
    recall(current->parent);
  }
}

template<size_t height>
void OcTree<height>::split(shared_ptr<Octant> &node, size_t current_level) {
  if (current_level == height) { return; }
  node->color = INTERVAL_NODE;
  size_t i = size_t(0);
  for (int x = 0; x < 2; x++) {
    for (int y = 0; y < 2; y++) {
      for (int z = 0; z < 2; z++) {
        Vector3d gen = node->child_size * Vector3d(double(x), double(y), double(z));
        node->children[i] = make_shared<Octant>(node->bound_min + gen, node->child_size, node);
        split(node->children[i], current_level + size_t(1));
        ++i;
      }
    }
  }
}

template<size_t height>
void OcTree<height>::recall(shared_ptr<Octant> &node, COLOR condition) {
  for (size_t i = size_t(0); i < NUM_CHILDS; ++i) {
    if (node->children[i]->color != condition) return;
  }
  size_t ind = size_t(0);
  for (size_t i = size_t(0); i < NUM_CHILDS; ++i) {
    if (condition == BLACK_NODE) {
      node->content.resize(node->content.size() + node->children[i]->content.size());
      for (shared_ptr<Vector3d> &p : node->children[i]->content) {
        node->content[ind++] = p;
      }
      node->children[i]->content.clear();
    }
  }
  node->color = condition;
  recall(node->parent);
}

template<size_t height>
void OcTree<height>::read_OcTree(shared_ptr<Octant> node) {
  for (size_t i = size_t(0); i < NUM_CHILDS; ++i) {
    if (node->children[i]->color == INTERVAL_NODE) {
      read_OcTree(node->children[i]);
    }
    else {
      if (node->children[i]->color == BLACK_NODE) {
        DrawableOctant cube_node(node->middle, node->own_size, 0);
        d_octants.push_back(cube_node);
      }
    }
  }
}