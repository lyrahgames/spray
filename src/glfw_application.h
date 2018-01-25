#ifndef SPRAY_GLFW_APPLICATION_H_
#define SPRAY_GLFW_APPLICATION_H_

#include <GLFW/glfw3.h>

class Glfw_application {
 public:
  Glfw_application();
  ~Glfw_application();
  Glfw_application(const Glfw_application&) = delete;
  Glfw_application& operator=(const Glfw_application&) = delete;
  Glfw_application(Glfw_application&&) = delete;
  Glfw_application&& operator=(Glfw_application&&) = delete;

  void execute();

 private:
  void render();

  GLFWwindow* window_;
};

#endif  // SPRAY_GLFW_APPLICATION_H_