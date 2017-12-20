#ifndef SPRAY_VIEWER_GLUT_APP_H_
#define SPRAY_VIEWER_GLUT_APP_H_

#include <Eigen/Dense>
#include <chrono>
#include <iomanip>
#include <iostream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <spray/spray.h>

namespace glut_app {

constexpr unsigned char glut_key_esc = 27;
constexpr unsigned char glut_key_b = 98;
constexpr unsigned char glut_key_g = 103;
constexpr unsigned char glut_key_w = 119;

constexpr float eye_azimuth_key_inc = 0.1f;
constexpr float eye_azimuth_inc_scale = 0.01f;
constexpr float eye_altitude_key_inc = 0.1f;
constexpr float eye_altitude_inc_scale = 0.01f;
constexpr float eye_altitude_max_abs = M_PI_2 - 0.0001f;
constexpr float eye_distance_inc_scale = 0.02f;
constexpr float eye_distance_min = 0.00001f;

struct state {
  int key_modifiers;
  int old_mouse_x;
  int old_mouse_y;
  spray::ray_tracer::orthonormal_frame world;
  float eye_distance;
  float eye_altitude;
  float eye_azimuth;
  bool opengl_rendering;
  Eigen::Vector3f clear_color{0.0f, 0.0f, 0.0f};
  spray::chrono::fps_meter fps_meter;
};

extern state data;
extern spray::ray_tracer::kernel kernel;

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

void render_with_ray_tracer();
void render_with_opengl();
void compute_camera_frame();

}  // namespace glut_app

#endif  // SPRAY_VIEWER_GLUT_APP_H_