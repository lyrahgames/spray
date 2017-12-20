cmake_minimum_required(VERSION 3.9)

project(spray VERSION 0.1)

add_executable(spray
  src/main.cc
  src/glut_app.cc
  src/scene.cc
  src/camera.cc
  src/intersection.cc
  src/fps_meter.cc
  src/orthonormal_frame.cc
  src/aabb.cc src/kernel.cc
)

set_property(TARGET spray PROPERTY CXX_STANDARD 14)

if (WIN32)
  target_compile_definitions(spray PRIVATE _USE_MATH_DEFINES)
endif()

find_package(Eigen3 3.3 REQUIRED NO_MODULE)
target_link_libraries(spray Eigen3::Eigen)

find_package(OpenGL REQUIRED)
target_link_libraries(spray OpenGL::GL OpenGL::GLU)

find_package(GLUT REQUIRED)
target_link_libraries(spray GLUT::GLUT)

find_package(OpenMP)
if (OPENMP_CXX_FOUND)
  target_link_libraries(spray OpenMP::OpenMP_CXX)
endif()