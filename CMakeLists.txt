cmake_minimum_required(VERSION 3.7.2)

project(spray VERSION 0.1)

add_executable(spray src/main.cc src/glut_app.cc src/scene.cc src/camera.cc src/intersection.cc src/fps_meter.cc src/orthonormal_frame.cc)

set_property(TARGET spray PROPERTY CXX_STANDARD 14)

find_package(Eigen3 3.3 REQUIRED NO_MODULE)
target_link_libraries(spray Eigen3::Eigen)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED FreeGLUT)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
target_link_libraries(spray ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")