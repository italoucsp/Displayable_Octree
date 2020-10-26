#include <Octree.hpp>

int main(int argc, char **argv){
  OcTree<5> demo("teapot.obj");
  demo.read_OcTree(demo.getBegin());
  runOPENGL(argc, argv);
  return 0;
}