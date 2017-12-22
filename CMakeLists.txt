cmake_minimum_required(VERSION 3.9)

project(spray_viewer VERSION 0.1)

add_subdirectory(src/spray)

add_executable(spray_viewer
  src/main.cc
  src/glut_app.cc
)

target_link_libraries(spray_viewer PUBLIC spray)

set_property(TARGET spray_viewer PROPERTY CXX_STANDARD 14)

if (WIN32)
  target_compile_definitions(spray_viewer PRIVATE _USE_MATH_DEFINES)
endif()

find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

target_link_libraries(spray_viewer PRIVATE
  Eigen3::Eigen
  OpenGL::GL
  OpenGL::GLU
  GLUT::GLUT
)
