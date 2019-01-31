#include "glut_application.h"
#include <Eigen/Dense>
#include <iomanip>
#include <iostream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <spray/spray.h>

namespace {

// constants
constexpr unsigned char glut_key_esc = 27;
constexpr float eye_azimuth_key_inc = 0.1f;
constexpr float eye_azimuth_inc_scale = 0.01f;
constexpr float eye_altitude_key_inc = 0.1f;
constexpr float eye_altitude_inc_scale = 0.01f;
constexpr float eye_altitude_max_abs = M_PI_2 - 0.0001f;
constexpr float eye_distance_inc_scale = 0.02f;
constexpr float eye_distance_min = 0.00001f;

// static variables
int key_modifiers{};
int old_mouse_x{};
int old_mouse_y{};
spray::Orthonormal_frame world{};
float eye_distance = 1.0f;
float eye_altitude = 0.0f;
float eye_azimuth = 0.0f;
bool opengl_rendering = false;
spray::Fps_meter fps_meter{3.0f};
spray::Kernel kernel{};
spray::Scene scene{};
spray::Camera camera{};
spray::Binary_bvh bvh{};

// function declarations
void initialize(int argc, char** argv);
// void close();
void render();
void resize(int width, int height);
// void idle();
void process_normal_keys(unsigned char key, int x, int y);
void process_special_keys(int key, int x, int y);
void process_mouse_buttons(int button, int button_state, int x, int y);
void process_mouse_move(int x, int y);
void process_passive_mouse_move(int x, int y);
// void process_mouse_wheel(int wheel, int direction, int x, int y);
void render_with_ray_tracer();
void render_with_opengl();
void compute_camera_frame();

// function implementations
void initialize(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  // glutInitWindowPosition(100, 100);
  glutInitWindowSize(camera.screen_width(), camera.screen_height());
  glutCreateWindow("spray: simulated particle ray tracer");

  glutDisplayFunc(render);
  glutReshapeFunc(resize);
  // glutIdleFunc(idle);
  glutIdleFunc(render);
  glutKeyboardFunc(process_normal_keys);
  glutSpecialFunc(process_special_keys);
  glutMouseFunc(process_mouse_buttons);
  glutPassiveMotionFunc(process_passive_mouse_move);
  glutMotionFunc(process_mouse_move);
  // glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
  // GLUT_ACTION_GLUTMAINLOOP_RETURNS);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // initializing scene and camera
  if (argc != 2) {
    std::cout << "usage: spray <file name>" << std::endl;
    exit(0);
  }

  const std::string file_path{argv[1]};

  const auto begin_file_extension = file_path.find_last_of('.');
  if (begin_file_extension == file_path.size())
    throw std::runtime_error("Files without extension are not supported.");

  const std::string file_extension =
      file_path.substr(file_path.find_last_of('.') + 1);

  if (file_extension == "obj" || file_extension == "OBJ") {
    spray::Obj_loader obj{argv[1]};
    std::cout << "vertex count = " << obj.vertex_data.size() << std::endl;
    std::cout << "normal count = " << obj.normal_data.size() << std::endl;
    std::cout << "uv count = " << obj.uv_data.size() << std::endl;
    std::cout << "face count = " << obj.face_data.size() << std::endl;
    scene = obj();
  } else if (file_extension == "stl" || file_extension == "STL") {
    scene = spray::Scene(argv[1]);
  } else {
    throw std::runtime_error("This file type is not supported.");
  }

  bvh = spray::Binary_bvh(scene);

  spray::Bounding_box bounding_box = spray::bounds(scene);

  camera.set_screen_resolution(320, 320);
  camera.set_field_of_view(0.5f * M_PI);
  const float scene_radius = spray::radius(bounding_box);
  eye_distance = scene_radius / std::sin(0.5f * camera.field_of_view());
  world = spray::blender_orthonormal_frame(spray::center(bounding_box));

  kernel.set_camera(&camera);
  kernel.set_scene(&scene);
  kernel.set_bvh(&bvh);
  kernel.reset_cache();

  compute_camera_frame();

  std::cout << "primitive count: " << scene.primitive_data().size()
            << std::endl;
  std::cout << "bvh node count: " << bvh.node_data().size() << std::endl;
  std::cout << "scene min: " << bounding_box.min().transpose() << std::endl
            << "scene max: " << bounding_box.max().transpose() << std::endl
            << "scene radius: " << spray::radius(bounding_box) << std::endl;
  std::cout << "world origin: " << world.origin().transpose() << std::endl;
  std::cout << "camera distance: " << eye_distance << std::endl;
}

void resize(int width, int height) {
  camera.set_screen_resolution(width, height);
  compute_camera_frame();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, camera.screen_width(), camera.screen_height());
  gluPerspective(camera.opengl_field_of_view(), camera.aspect_ratio(), 0.1f,
                 1000.0f);
  glMatrixMode(GL_MODELVIEW);
}

void render() {
  fps_meter.measure();

  if (opengl_rendering) {
    render_with_opengl();
  } else {
    render_with_ray_tracer();
  }

  glutSwapBuffers();
}

// void idle() {}

void process_normal_keys(unsigned char key, int x, int y) {
  switch (key) {
    case glut_key_esc:  // exit the application
      exit(0);
      break;

    case 'b':  // set to blender coordinate system
      eye_azimuth = eye_altitude = 0.0f;
      world = spray::blender_orthonormal_frame(world.origin());
      break;

    case 'g':  // set to opengl coordinate system
      eye_azimuth = eye_altitude = 0.0f;
      world = spray::opengl_orthonormal_frame(world.origin());
      break;

    case 'w':  // toggle rendering of wireframe via opengl
      opengl_rendering = !opengl_rendering;
      break;
  }

  compute_camera_frame();
}

void process_special_keys(int key, int x, int y) {
  switch (key) {
    case GLUT_KEY_LEFT:
      eye_azimuth += eye_azimuth_key_inc;
      break;
    case GLUT_KEY_RIGHT:
      eye_azimuth -= eye_azimuth_key_inc;
      break;
    case GLUT_KEY_UP:
      eye_altitude += eye_altitude_key_inc;
      break;
    case GLUT_KEY_DOWN:
      eye_altitude -= eye_altitude_key_inc;
      break;
  }
  compute_camera_frame();
  glutPostRedisplay();
}

void process_mouse_buttons(int button, int button_state, int x, int y) {
  key_modifiers = glutGetModifiers();
}

void process_mouse_move(int x, int y) {
  const float x_difference = static_cast<float>(x - old_mouse_x);
  const float y_difference = static_cast<float>(y - old_mouse_y);

  switch (key_modifiers) {
    // zooming
    case GLUT_ACTIVE_CTRL:
      eye_distance += eye_distance_inc_scale * y_difference * eye_distance;
      if (eye_distance < eye_distance_min) eye_distance = eye_distance_min;
      break;

    // moving
    case GLUT_ACTIVE_SHIFT:
      world.origin(world.origin() -
                   camera.frame().right() * x_difference * camera.pixel_size() *
                       eye_distance +
                   camera.frame().up() * y_difference * camera.pixel_size() *
                       eye_distance);
      break;

    // rotating
    default:
      eye_azimuth -= x_difference * eye_azimuth_inc_scale;
      eye_altitude += y_difference * eye_altitude_inc_scale;
      if (eye_altitude > eye_altitude_max_abs)
        eye_altitude = eye_altitude_max_abs;
      if (eye_altitude < -eye_altitude_max_abs)
        eye_altitude = -eye_altitude_max_abs;
  }

  compute_camera_frame();

  old_mouse_x = x;
  old_mouse_y = y;
}

void process_passive_mouse_move(int x, int y) {
  old_mouse_x = x;
  old_mouse_y = y;
}

void compute_camera_frame() {
  camera.look_at(spray::horizontal_coordinates(world, eye_distance,
                                               eye_altitude, eye_azimuth),
                 world.origin(), world.up());
  kernel.reset_cache();

  glLoadIdentity();
  gluLookAt(camera.position()(0), camera.position()(1), camera.position()(2),
            world.origin()(0), world.origin()(1), world.origin()(2),
            world.up()(0), world.up()(1), world.up()(2));
}

void render_with_ray_tracer() {
  kernel.render_bvh();
  glDrawPixels(camera.screen_width(), camera.screen_height(), GL_RGBA, GL_FLOAT,
               kernel.pixel_buffer().data());
}

void render_with_opengl() {
  glClearColor(0, 0, 0, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (int i = 0; i < static_cast<int>(scene.primitive_data().size()); ++i) {
    const spray::Scene::primitive& p = scene.primitive_data()[i];

    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3fv(reinterpret_cast<const GLfloat*>(
        scene.vertex_data()[p.vertex_id[0]].position.data()));
    glVertex3fv(reinterpret_cast<const GLfloat*>(
        scene.vertex_data()[p.vertex_id[1]].position.data()));
    glVertex3fv(reinterpret_cast<const GLfloat*>(
        scene.vertex_data()[p.vertex_id[2]].position.data()));
    glEnd();
  }
}

}  // namespace

Glut_application* Glut_application::instance_ = nullptr;

Glut_application::Glut_application(int argc, char** argv) {
  if (instance_)
    throw std::runtime_error(
        "Glut_application could not be created, because there already exists "
        "an instance!");

  initialize(argc, argv);
  instance_ = this;
}

Glut_application::Glut_application() : Glut_application(0, nullptr) {}

Glut_application::~Glut_application() { instance_ = nullptr; }

void Glut_application::execute() { glutMainLoop(); }