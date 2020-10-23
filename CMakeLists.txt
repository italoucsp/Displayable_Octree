project("Oc-Tree")

cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories($ENV{GLEW_INCLUDE_DIR})
include_directories($ENV{GLFW_INCLUDE_DIR})

link_directories($ENV{GLEW_LIBRARY})
link_directories($ENV{GLFW_LIBRARY})

find_package(OpenGL REQUIRED)

include_directories($(CMAKE_CURRENT_SOURCE_DIR)/source)

add_executable(OcTree_project source/main.cpp)

target_link_libraries(OcTree_project ${OPENGL_gl_LIBRARY} glew32 glfw3dll)