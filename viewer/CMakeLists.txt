find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(Eigen3 REQUIRED NO_MODULE)

add_executable(spray-view
  main.cc
  viewer.h
  viewer.cc
  camera.cc
)

target_link_libraries(spray-view
  PRIVATE
    Qt5::Widgets
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
    Eigen3::Eigen
    spray::spray
)

set_target_properties(spray-view
  PROPERTIES
    AUTOMOC TRUE
)