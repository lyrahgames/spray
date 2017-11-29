#ifndef SPRAY_GLUT_APP_H_
#define SPRAY_GLUT_APP_H_

#include <Eigen/Dense>
#include <chrono>
#include <iomanip>
#include <iostream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "fps_meter.h"
#include "kernel.h"
#include "orthonormal_frame.h"
#include "scene.h"

namespace spray {
namespace glut_app {

constexpr unsigned char glut_key_esc = 27;

struct state {
  int key_modifiers;
  int old_mouse_x;
  int old_mouse_y;
  ray_tracer::scene scene;
  ray_tracer::kernel rtkernel;
  fps_meter fpsm;
  ray_tracer::orthonormal_frame world;
  float camera_distance;
  float camera_altitude;
  float camera_azimuth;
};

extern state data;

void init(int argc, char** argv);
void exec();

void close();
void render();
void resize(int width, int height);
void idle();
void process_normal_keys(unsigned char key, int x, int y);
void process_special_keys(int key, int x, int y);
void process_mouse_buttons(int button, int button_state, int x, int y);
void process_mouse_move(int x, int y);
void process_passive_mouse_move(int x, int y);
void process_mouse_wheel(int wheel, int direction, int x, int y);

}  // namespace glut_app
}  // namespace spray

#endif  // SPRAY_GLUT_APP_H_