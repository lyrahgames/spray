#ifndef SPRAY_GLUT_APP_H_
#define SPRAY_GLUT_APP_H_

#include <iostream>

#ifdef __APPLE__
#include <GLUT/freeglut.h>
#else
#include <GL/freeglut.h>
#endif

namespace spray {
namespace glut_app {

constexpr unsigned char glut_key_esc = 27;

struct state_t {
  float angle;
};

extern state_t state;

void init(int argc, char** argv);
void exec();

void close();
void render();
void resize(int width, int height);
void process_normal_keys(unsigned char key, int x, int y);
void process_special_keys(int key, int x, int y);

}  // namespace glut_app
}  // namespace spray

#endif  // SPRAY_GLUT_APP_H_