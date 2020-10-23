#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <time.h>
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

#define CCOLORG 0.5f,0.5f,0.5f
#define CCOLORN 0.f,0.f,0.f

double degX = 0.f, degY = 0.f, degZ = 0.f;
double zoom = -500;

const int W = 800;
const int H = 650;

bool viewMode = 1;

struct Point {
  double x, y, z;
  Point() { x = y = z = 0.0; }
  Point(double X, double Y, double Z) {
    x = X; y = Y; z = Z;
  }
  bool operator==(Point &p) {
    return x == p.x && y == p.y && z == p.z;
  }
  Point operator+(Point p2) {
    Point p;
    p.x = x + p2.x; p.y = y + p2.y; p.z = z + p2.z;
    return p;
  }
  Point operator-(Point p2) {
    Point p;
    p.x = x - p2.x; p.y = y - p2.y; p.z = z - p2.z;
    return p;
  }
  Point operator*(Point p2) {
    Point p;
    p.x = x * p2.x; p.y = y * p2.y; p.z = z * p2.z;
    return p;
  }
  Point operator*(double xyz) {
    Point p;
    p.x = x * xyz; p.y = y * xyz; p.z = z * xyz;
    return p;
  }
  Point& operator=(const Point &p) {
    this->x = p.x; this->y = p.y; this->z = p.z;
    return *this;
  }

  void print() {
    cout << '<' << x << ',' << y << ',' << z << '>' << endl;
  }
};

void drawFilledCube(Point pos0, Point dim) {
  glBegin(GL_QUADS);

  glColor3d(CCOLORG);
  glVertex3f((GLfloat)pos0.x, (GLfloat)pos0.y, (GLfloat)pos0.z);
  glVertex3f((GLfloat)pos0.x + (GLfloat)dim.x, (GLfloat)pos0.y, (GLfloat)pos0.z);
  glVertex3f((GLfloat)pos0.x + (GLfloat)dim.x, (GLfloat)pos0.y + (GLfloat)dim.y, (GLfloat)pos0.z);
  glVertex3f((GLfloat)pos0.x, (GLfloat)pos0.y + (GLfloat)dim.y, (GLfloat)pos0.z);

  glVertex3f((GLfloat)pos0.x, (GLfloat)pos0.y, (GLfloat)pos0.z + (GLfloat)dim.z);
  glVertex3f((GLfloat)pos0.x + (GLfloat)dim.x, (GLfloat)pos0.y, (GLfloat)pos0.z + (GLfloat)dim.z);
  glVertex3f((GLfloat)pos0.x + (GLfloat)dim.x, (GLfloat)pos0.y + (GLfloat)dim.y, (GLfloat)pos0.z + (GLfloat)dim.z);
  glVertex3f((GLfloat)pos0.x, (GLfloat)pos0.y + (GLfloat)dim.y, (GLfloat)pos0.z + (GLfloat)dim.z);

  glVertex3f((GLfloat)pos0.x, (GLfloat)pos0.y, (GLfloat)pos0.z);
  glVertex3f((GLfloat)pos0.x, (GLfloat)pos0.y, (GLfloat)pos0.z + (GLfloat)dim.z);
  glVertex3f((GLfloat)pos0.x, (GLfloat)pos0.y + (GLfloat)dim.y, (GLfloat)pos0.z + (GLfloat)dim.z);
  glVertex3f((GLfloat)pos0.x, (GLfloat)pos0.y + (GLfloat)dim.y, (GLfloat)pos0.z);

  glVertex3f((GLfloat)pos0.x + (GLfloat)dim.x, (GLfloat)pos0.y, (GLfloat)pos0.z);
  glVertex3f((GLfloat)pos0.x + (GLfloat)dim.x, (GLfloat)pos0.y, (GLfloat)pos0.z + (GLfloat)dim.z);
  glVertex3f((GLfloat)pos0.x + (GLfloat)dim.x, (GLfloat)pos0.y + (GLfloat)dim.y, (GLfloat)pos0.z + (GLfloat)dim.z);
  glVertex3f((GLfloat)pos0.x + (GLfloat)dim.x, (GLfloat)pos0.y + (GLfloat)dim.y, (GLfloat)pos0.z);

  glVertex3f((GLfloat)pos0.x, (GLfloat)pos0.y, (GLfloat)pos0.z);
  glVertex3f((GLfloat)pos0.x + (GLfloat)dim.x, (GLfloat)pos0.y, (GLfloat)pos0.z);
  glVertex3f((GLfloat)pos0.x + (GLfloat)dim.x, (GLfloat)pos0.y, (GLfloat)pos0.z + (GLfloat)dim.z);
  glVertex3f((GLfloat)pos0.x, (GLfloat)pos0.y, (GLfloat)pos0.z + (GLfloat)dim.z);

  glVertex3f((GLfloat)pos0.x, (GLfloat)pos0.y + (GLfloat)dim.y, (GLfloat)pos0.z);
  glVertex3f((GLfloat)pos0.x + (GLfloat)dim.x, (GLfloat)pos0.y + (GLfloat)dim.y, (GLfloat)pos0.z);
  glVertex3f((GLfloat)pos0.x + (GLfloat)dim.x, (GLfloat)pos0.y + (GLfloat)dim.y, (GLfloat)pos0.z + (GLfloat)dim.z);
  glVertex3f((GLfloat)pos0.x, (GLfloat)pos0.y + (GLfloat)dim.y, (GLfloat)pos0.z + (GLfloat)dim.z);
  glEnd();
}

void drawSkelethonCube(Point pos0, Point dim) {
  glBegin(GL_LINE_LOOP);
  glColor3d(CCOLORN);
  glVertex3f((GLfloat)pos0.x, (GLfloat)pos0.y, (GLfloat)pos0.z);
  glVertex3f((GLfloat)pos0.x + (GLfloat)dim.x, (GLfloat)pos0.y, (GLfloat)pos0.z);
  glVertex3f((GLfloat)pos0.x + (GLfloat)dim.x, (GLfloat)pos0.y + (GLfloat)dim.y, (GLfloat)pos0.z);
  glVertex3f((GLfloat)pos0.x, (GLfloat)pos0.y + (GLfloat)dim.y, (GLfloat)pos0.z);
  glEnd();

  glBegin(GL_LINE_LOOP);
  glColor3d(CCOLORN);
  glVertex3f((GLfloat)pos0.x, (GLfloat)pos0.y, (GLfloat)pos0.z + (GLfloat)dim.z);
  glVertex3f((GLfloat)pos0.x + (GLfloat)dim.x, (GLfloat)pos0.y, (GLfloat)pos0.z + (GLfloat)dim.z);
  glVertex3f((GLfloat)pos0.x + (GLfloat)dim.x, (GLfloat)pos0.y + (GLfloat)dim.y, (GLfloat)pos0.z + (GLfloat)dim.z);
  glVertex3f((GLfloat)pos0.x, (GLfloat)pos0.y + (GLfloat)dim.y, (GLfloat)pos0.z + (GLfloat)dim.z);
  glEnd();

  glBegin(GL_LINES);
  glColor3d(CCOLORN);
  glVertex3f((GLfloat)pos0.x, (GLfloat)pos0.y, (GLfloat)pos0.z);
  glVertex3f((GLfloat)pos0.x, (GLfloat)pos0.y, (GLfloat)pos0.z + (GLfloat)dim.z);
  glEnd();

  glBegin(GL_LINES);
  glColor3d(CCOLORN);
  glVertex3f((GLfloat)pos0.x + (GLfloat)dim.x, (GLfloat)pos0.y, (GLfloat)pos0.z);
  glVertex3f((GLfloat)pos0.x + (GLfloat)dim.x, (GLfloat)pos0.y, (GLfloat)pos0.z + (GLfloat)dim.z);
  glEnd();

  glBegin(GL_LINES);
  glColor3d(CCOLORN);
  glVertex3f((GLfloat)pos0.x + (GLfloat)dim.x, (GLfloat)pos0.y + (GLfloat)dim.y, (GLfloat)pos0.z);
  glVertex3f((GLfloat)pos0.x + (GLfloat)dim.x, (GLfloat)pos0.y + (GLfloat)dim.y, (GLfloat)pos0.z + (GLfloat)dim.z);
  glEnd();

  glBegin(GL_LINES);
  glColor3d(CCOLORN);
  glVertex3f((GLfloat)pos0.x, (GLfloat)pos0.y + (GLfloat)dim.y, (GLfloat)pos0.z);
  glVertex3f((GLfloat)pos0.x, (GLfloat)pos0.y + (GLfloat)dim.y, (GLfloat)pos0.z + (GLfloat)dim.z);
  glEnd();
}

void drawCube(Point origin, Point dim) {
  GLfloat centerPosX = (GLfloat)origin.x, centerPosY = (GLfloat)origin.y, centerPosZ = (GLfloat)origin.z;
  GLfloat dimX = (GLfloat)dim.x * 0.5f;
  GLfloat dimY = (GLfloat)dim.y * 0.5f;
  GLfloat dimZ = (GLfloat)dim.z * 0.5f;

  GLfloat vertices[] =
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

    // top face
    centerPosX - dimX, centerPosY - dimY, centerPosZ + dimZ, // top left
    centerPosX - dimX, centerPosY - dimY, centerPosZ - dimZ, // top right
    centerPosX + dimX, centerPosY - dimY, centerPosZ - dimZ, // bottom right
    centerPosX + dimX, centerPosY - dimY, centerPosZ + dimZ  // bottom left
  };

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glColor3f(0.f, 0.f, 0.f);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, vertices);

  glDrawArrays(GL_QUADS, 0, 24);

  glDisableClientState(GL_VERTEX_ARRAY);
}

struct DrawableOctant {
  Point startP, dim;
  bool isRoot;
  DrawableOctant(Point boundMin, Point boundMax, bool isRoot = false) {
    dim = boundMax - boundMin;
    startP = boundMin;
    this->isRoot = isRoot;
  }
  void show() {
    if (viewMode) {
      if (!isRoot) {
        drawFilledCube(startP, dim);
      }
      drawSkelethonCube(startP, dim);
    }
    else {
      if (!isRoot) {
        drawFilledCube(startP, dim);
        drawSkelethonCube(startP, dim);
      }
    }
  }
};

static vector<DrawableOctant> d_octants;

void show_tree(GLFWwindow *screen) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
  glTranslated(W / 2, H / 2, -500);
  glRotated(degX, 1, 0, 0);
  glRotated(degY, 0, 1, 0);
  glRotated(degZ, 0, 0, 1);
  glTranslated(-W / 2, -H / 2, 500);

  for (auto &o : d_octants) {
    o.show();
  }
  drawCube(Point(W / 2, H / 2,-500),Point(50,50,70));
  glPopMatrix();
}

static void key_callback(GLFWwindow *screen, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(screen, GLFW_TRUE);

  const GLfloat rotationSpeed = 10;

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
      zoom += 0.1f;
      break;
    case GLFW_KEY_KP_SUBTRACT:
      zoom -= 0.1f;
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
}