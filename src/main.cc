#include <iostream>
#include "glut_app.h"

int main(int argc, char** argv) {
  std::cout << "spray: simulated particle ray tracing" << std::endl
            << std::endl;

  spray::glut_app::init(argc, argv);
  spray::glut_app::exec();

  std::cout << "quit" << std::endl;
}
