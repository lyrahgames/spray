#include "glfw_application.h"
#include <stdexcept>

Glfw_application::Glfw_application() {
  if (!glfwInit()) throw std::runtime_error("GLFW could not be initialized!");

  if (!(window_ =
            glfwCreateWindow(640, 480, "GLFW Application", nullptr, nullptr))) {
    glfwTerminate();
    throw std::runtime_error(
        "Window of Glfw_application could not be created!");
  }
}

Glfw_application::~Glfw_application() {
  glfwDestroyWindow(window_);
  glfwTerminate();
}

void Glfw_application::execute() {
  glfwMakeContextCurrent(window_);
  while (!glfwWindowShouldClose(window_)) {
    render();
    glfwSwapBuffers(window_);
    glfwPollEvents();
  }
}

void Glfw_application::render() {
  glClearColor(0.5, 0.5, 0.5, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
}