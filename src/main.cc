#include <iostream>
#include "glut_app.h"

int main(int argc, char** argv) {
  // std::cout << "FreeGLUT test run..." << std::flush;

  spray::glut_app::init(argc, argv);
  spray::glut_app::exec();

  // std::cout << "done." << std::endl;
}
