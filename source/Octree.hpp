#include <octree_utils.hpp>

template<size_t fill_condition>
class OcTree {
public:
  OcTree(string path_file);
  ~OcTree();
private:
  struct Octant {
    shared_ptr<Octant> children[8] = {0};
  };
};