#include <iostream>
#include "glut_application.h"

int main(int argc, char** argv) {
  std::cout << "spray: simulated particle ray tracing" << std::endl
            << std::endl;

  Glut_application application(argc, argv);
  application.execute();
}
