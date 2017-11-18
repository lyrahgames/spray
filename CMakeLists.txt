cmake_minimum_required(VERSION 3.7.2)

project(spray VERSION 0.1)

add_executable(spray src/main.cc)

find_package(Eigen3 3.3 REQUIRED NO_MODULE)
target_link_libraries(spray Eigen3::Eigen)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
target_link_libraries(spray ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})