cmake_minimum_required(VERSION 3.10)
project(spray LANGUAGES CXX)
enable_testing()

add_subdirectory(spray)
add_subdirectory(tests)
add_subdirectory(viewer)

find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_executable(spray_viewer
  main.cc
  glut_application.cc
)

if (WIN32)
  target_compile_definitions(spray_viewer
    PRIVATE
      _USE_MATH_DEFINES
  )
endif()

target_link_libraries(spray_viewer
  PRIVATE
    spray::spray
    Eigen3::Eigen
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
)