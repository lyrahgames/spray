cmake_minimum_required(VERSION 3.9)

project(spray.ray_tracer VERSION 0.1)

add_library(spray.ray_tracer STATIC
  scene.cc
  camera.cc
  intersection.cc
  orthonormal_frame.cc
  bounding_box.cc
  kernel.cc
)

set_property(TARGET spray.ray_tracer PROPERTY CXX_STANDARD 14)

if (WIN32)
  target_compile_definitions(spray.ray_tracer PRIVATE _USE_MATH_DEFINES)
endif()

find_package(Eigen3 3.3 REQUIRED NO_MODULE)
target_link_libraries(spray.ray_tracer PUBLIC Eigen3::Eigen)

find_package(OpenMP)
if (OPENMP_CXX_FOUND)
  target_link_libraries(spray.ray_tracer PRIVATE OpenMP::OpenMP_CXX)
endif()

add_library(spray::ray_tracer ALIAS spray.ray_tracer)