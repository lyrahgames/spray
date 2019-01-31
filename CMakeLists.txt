cmake_minimum_required(VERSION 3.9)

project(spray_viewer VERSION 0.1)

#set(BOOST_USE_STATIC_LIBS ON)
#set(BOOST_USE_MULTITHREADED ON)

add_subdirectory(spray)
add_subdirectory(tests)

add_subdirectory(viewer)

add_executable(spray_viewer
  main.cc
  glut_application.cc
)

target_link_libraries(spray_viewer PUBLIC spray)

set_property(TARGET spray_viewer PROPERTY CXX_STANDARD 14)

if (WIN32)
  target_compile_definitions(spray_viewer PRIVATE _USE_MATH_DEFINES)
endif()

find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(glfw3)

target_link_libraries(spray_viewer PRIVATE
  Eigen3::Eigen
  OpenGL::GL
  OpenGL::GLU
  GLUT::GLUT
)

if (glfw3_FOUND)
  add_executable(glfw_viewer
    glfw_application.cc
    glfw_viewer.cc
  )

  target_link_libraries(glfw_viewer PRIVATE
    glfw
    OpenGL::GL
    OpenGL::GLU
  )
endif()