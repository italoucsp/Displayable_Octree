#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <time.h>
#include <memory>
#include <stdexcept>
#include <array>
#include <utility>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

#define CCOLORG 0.35f,0.5f,0.45f
#define CCOLORN 0.f,0.f,0.f
#define ERROR_FILL_VALUE "ERROR: Fill condition value must be between 1 and 100."
#define ERROR_FILE_NOT_FOUND "ERROR: The file can not be openned."
#define ALERT(message) cout << message << endl;

double degX = 20.f, degY = 20.f, degZ = 0.f;
double zoom = 1, scale = 10;

const int W = 800;
const int H = 650;

bool viewMode = 1;//1 = filled, 0 = skelethon

struct Vector3d {
  double x, y, z;
  Vector3d() { x = y = z = 0.0; }
  Vector3d(double X, double Y, double Z) {
    x = X; y = Y; z = Z;
  }
  bool operator==(Vector3d &p) {
    return x == p.x && y == p.y && z == p.z;
  }
  bool operator<(Vector3d &p) {
    return x < p.x && y < p.y && z < p.z;
  }
  bool operator<=(Vector3d &p) {
    return x <= p.x && y <= p.y && z <= p.z;
  }
  Vector3d operator+(Vector3d p2) {
    Vector3d p;
    p.x = x + p2.x; p.y = y + p2.y; p.z = z + p2.z;
    return p;
  }
  Vector3d operator-(Vector3d p2) {
    Vector3d p;
    p.x = x - p2.x; p.y = y - p2.y; p.z = z - p2.z;
    return p;
  }
  Vector3d operator*(Vector3d p2) {
    Vector3d p;
    p.x = x * p2.x; p.y = y * p2.y; p.z = z * p2.z;
    return p;
  }
  Vector3d operator*(double xyz) {
    Vector3d p;
    p.x = x * xyz; p.y = y * xyz; p.z = z * xyz;
    return p;
  }
  Vector3d& operator=(const Vector3d &p) {
    this->x = p.x; this->y = p.y; this->z = p.z;
    return *this;
  }

  void print() {
    cout << '<' << x << ',' << y << ',' << z << '>' << endl;
  }
};

Vector3d center = Vector3d(double(W) / 2, double(H) / 2, -500.0);

Vector3d root_pos;

void drawCube(Vector3d origin, Vector3d dim, bool filled) {
  GLfloat centerPosX = (GLfloat)origin.x, centerPosY = (GLfloat)origin.y, centerPosZ = (GLfloat)origin.z;
  GLfloat dimX = (GLfloat)dim.x * 0.5f;
  GLfloat dimY = (GLfloat)dim.y * 0.5f;
  GLfloat dimZ = (GLfloat)dim.z * 0.5f;

  GLfloat cube_vertices[] =
  {
    // front face
    centerPosX - dimX, centerPosY + dimY, centerPosZ + dimZ, // top left
    centerPosX + dimX, centerPosY + dimY, centerPosZ + dimZ, // top right
    centerPosX + dimX, centerPosY - dimY, centerPosZ + dimZ, // bottom right
    centerPosX - dimX, centerPosY - dimY, centerPosZ + dimZ, // bottom left

    // back face
    centerPosX - dimX, centerPosY + dimY, centerPosZ - dimZ, // top left
    centerPosX + dimX, centerPosY + dimY, centerPosZ - dimZ, // top right
    centerPosX + dimX, centerPosY - dimY, centerPosZ - dimZ, // bottom right
    centerPosX - dimX, centerPosY - dimY, centerPosZ - dimZ, // bottom left

    // left face
    centerPosX - dimX, centerPosY + dimY, centerPosZ + dimZ, // top left
    centerPosX - dimX, centerPosY + dimY, centerPosZ - dimZ, // top right
    centerPosX - dimX, centerPosY - dimY, centerPosZ - dimZ, // bottom right
    centerPosX - dimX, centerPosY - dimY, centerPosZ + dimZ, // bottom left

    // right face
    centerPosX + dimX, centerPosY + dimY, centerPosZ + dimZ, // top left
    centerPosX + dimX, centerPosY + dimY, centerPosZ - dimZ, // top right
    centerPosX + dimX, centerPosY - dimY, centerPosZ - dimZ, // bottom right
    centerPosX + dimX, centerPosY - dimY, centerPosZ + dimZ, // bottom left

    // top face
    centerPosX - dimX, centerPosY + dimY, centerPosZ + dimZ, // top left
    centerPosX - dimX, centerPosY + dimY, centerPosZ - dimZ, // top right
    centerPosX + dimX, centerPosY + dimY, centerPosZ - dimZ, // bottom right
    centerPosX + dimX, centerPosY + dimY, centerPosZ + dimZ, // bottom left

    // bottom face
    centerPosX - dimX, centerPosY - dimY, centerPosZ + dimZ, // top left
    centerPosX - dimX, centerPosY - dimY, centerPosZ - dimZ, // top right
    centerPosX + dimX, centerPosY - dimY, centerPosZ - dimZ, // bottom right
    centerPosX + dimX, centerPosY - dimY, centerPosZ + dimZ,  // bottom left

    centerPosX - dimX, centerPosY + dimY, centerPosZ + dimZ, // top left
    centerPosX - dimX, centerPosY - dimY, centerPosZ + dimZ, // top right
    centerPosX + dimX, centerPosY - dimY, centerPosZ + dimZ, // bottom right
    centerPosX + dimX, centerPosY + dimY, centerPosZ + dimZ, // bottom left

    centerPosX - dimX, centerPosY + dimY, centerPosZ - dimZ, // top left
    centerPosX - dimX, centerPosY - dimY, centerPosZ - dimZ, // top right
    centerPosX + dimX, centerPosY - dimY, centerPosZ - dimZ, // bottom right
    centerPosX + dimX, centerPosY + dimY, centerPosZ - dimZ // bottom left
  };

  if (filled) {
    glColor3f(CCOLORG);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, cube_vertices);
    glDrawArrays(GL_QUADS, 0, 24);
    glDisableClientState(GL_VERTEX_ARRAY);
  }
  glColor3f(CCOLORN);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, cube_vertices);
  glDrawArrays(GL_LINES, 0, 32);
  glDisableClientState(GL_VERTEX_ARRAY);
}

Vector3d NormalizeToCenter(Vector3d point) {
  Vector3d normalized = point + center - root_pos * zoom;
  return normalized;
}

struct DrawableOctant {
  Vector3d startP, size;
  bool isRoot;
  DrawableOctant(Vector3d middle, Vector3d own_size, bool isRoot = false) {
    size = own_size;
    startP = middle;
    this->isRoot = isRoot;
    if (this->isRoot) {
      root_pos = middle;
    }
  }
  void show() {
    drawCube(NormalizeToCenter(startP * zoom), size * zoom, (!isRoot) && viewMode);//0 = only skelethon, 1 = filled
  }
};

static vector<DrawableOctant> d_octants;

void show_tree(GLFWwindow *screen) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
  glTranslated(center.x,center.y,center.z);
  glRotated(degX, 1, 0, 0);
  glRotated(degY, 0, 1, 0);
  glRotated(degZ, 0, 0, 1);
  glTranslated(-center.x, -center.y, -center.z);

  for (auto &o : d_octants) {
    o.show();
  }
  glPopMatrix();
}

static void key_callback(GLFWwindow *screen, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(screen, GLFW_TRUE);

  const GLfloat rotationSpeed = 5;

  if (action == GLFW_PRESS || action == GLFW_REPEAT)
  {
    switch (key)
    {
    case GLFW_KEY_UP:
      degX -= rotationSpeed;
      break;
    case GLFW_KEY_DOWN:
      degX += rotationSpeed;
      break;
    case GLFW_KEY_RIGHT:
      degY += rotationSpeed;
      break;
    case GLFW_KEY_LEFT:
      degY -= rotationSpeed;
      break;
    case GLFW_KEY_KP_ADD:
      zoom += 0.5f;
      break;
    case GLFW_KEY_KP_SUBTRACT:
      zoom -= 0.5f;
      break;
    case GLFW_KEY_KP_MULTIPLY:
      viewMode = !viewMode;
      break;
    }
  }
}

void runOPENGL(int argc, char **argv) {
  glfwInit();
  glewInit();
  GLFWwindow *screen = glfwCreateWindow(W, H, "Octree-Viz", nullptr, nullptr);
  glfwMakeContextCurrent(screen);

  GLint w, h;
  glfwGetFramebufferSize(screen, &w, &h);
  glViewport(0, 0, w, h);
  glfwSetKeyCallback(screen, key_callback);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, w, 0, h, 0, 1000);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  while (!glfwWindowShouldClose(screen)) {
    glfwPollEvents();

    glClearColor(1.f,1.f,1.f,1.f);

    show_tree(screen);

    glfwSwapBuffers(screen);
  }
  glfwTerminate();
}

void maxPoint(Vector3d &M, Vector3d&X) {
  M.x = (M.x < X.x) ? X.x : M.x;
  M.y = (M.y < X.y) ? X.y : M.y;
  M.z = (M.z < X.z) ? X.z : M.z;
}

void minPoint(Vector3d &M, Vector3d&X) {
  M.x = (M.x > X.x) ? X.x : M.x;
  M.y = (M.y > X.y) ? X.y : M.y;
  M.z = (M.z > X.z) ? X.z : M.z;
}

Vector3d StoPt(string line) {
  string s_dat[3] = { "","","" };
  int xyz = 0;
  for (int i = 2; i < int(line.length()); i++) {
    if (line[i] == ' ') {
      ++xyz;
      continue;
    }
    if (line[i] == '\n')break;
    s_dat[xyz].push_back(line[i]);
  }
  Vector3d p(stod(s_dat[0]), stod(s_dat[1]), stod(s_dat[2]));
  return p;
}

pair<Vector3d,Vector3d> get_Points(ifstream &obj3d_file, vector<Vector3d> &data_set) {
  string line;
  Vector3d maxVec, minVec;
  while (!obj3d_file.eof()) {
    getline(obj3d_file, line);
    if (line[0] == 'v' && line[1] == ' ') {
      Vector3d temp = StoPt(line) * scale;
      maxPoint(maxVec, temp);
      minPoint(minVec, temp);
      data_set.push_back(temp);
    }
    else if (line[0] == 'f') break;
  }
  maxVec = maxVec + Vector3d(1.0, 1.0, 1.0);
  minVec = minVec - Vector3d(1.0, 1.0, 1.0);
  ALERT("=> Region Octree MIN BOUND: "); minVec.print();
  ALERT("=> Region Octree MAX BOUND: "); maxVec.print();
  obj3d_file.close();
  return make_pair(minVec, maxVec);
}