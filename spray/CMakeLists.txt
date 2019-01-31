cmake_minimum_required(VERSION 3.10)
project(spray VERSION 0.1.0 LANGUAGES CXX)
set(PROJECT_NAMESPACE "${PROJECT_NAME}")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

# Dependencies
find_package(Eigen3 REQUIRED)
find_package(OpenMP)

# Building
add_library(${PROJECT_NAME}
  binary_bvh.cc
  bounding_box.cc
  camera.cc
  fps_meter.cc
  intersection.cc
  kernel.cc
  orthonormal_frame.cc
  scene.cc
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
)

target_compile_features(${PROJECT_NAME}
  PUBLIC
    cxx_std_17
)

target_link_libraries(spray
  PUBLIC
    Eigen3::Eigen
)

if (OPENMP_CXX_FOUND)
  target_link_libraries(spray
    PRIVATE
    OpenMP::OpenMP_CXX
  )
endif()

add_library(${PROJECT_NAMESPACE}::${PROJECT_NAME}
  ALIAS
    ${PROJECT_NAME}
)

# Deploying
# include(${PROJECT_NAME}Install)
# include(${PROJECT_NAME}PkgconfigInstall)
# include(${PROJECT_NAME}Uninstall)
# include(${PROJECT_NAME}Export)