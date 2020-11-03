#include <Octree.hpp>

int main(int argc, char **argv){
  OcTree<8> demo("Shelby.obj");
  demo.read_OcTree(demo.getBegin());
  runOPENGL(argc, argv);
  return 0;
}