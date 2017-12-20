#include <iostream>
#include "glut_app.h"

int main(int argc, char** argv) {
  std::cout << "spray: simulated particle ray tracing" << std::endl
            << std::endl;

  glut_app::init(argc, argv);
  glut_app::exec();

  std::cout << "quit" << std::endl;
}
