#include "glut_app.h"

namespace spray {
namespace glut_app {

state data{};

void init(int argc, char** argv) {
  data.fpsm = fps_meter(5.0f);

  if (argc != 2) {
    std::cout << "usage: spray <file name>" << std::endl;
    exit(0);
  }

  data.rtkernel.s = ray_tracer::load_stl(argv[1]);

  data.rtkernel.cam.set_screen_resolution(320, 320);
  data.rtkernel.cam.set_field_of_view(0.5f * M_PI);

  data.camera_altitude = 0.0f;
  data.camera_azimuth = 0.0f;
  data.camera_distance = 2.0f * data.rtkernel.s.radius /
                         tanf(0.5f * data.rtkernel.cam.field_of_view());

  data.world = ray_tracer::orthonormal_frame(data.rtkernel.s.center,
                                             Eigen::Vector3f(0, 1, 0),
                                             Eigen::Vector3f(1, 0, 0));

  std::cout << "primitive count:" << data.rtkernel.s.primitive_vector.size()
            << std::endl;
  std::cout << "scene min:" << data.rtkernel.s.min.transpose() << std::endl
            << "scene max:" << data.rtkernel.s.max.transpose() << std::endl
            << "scene radius:" << data.rtkernel.s.radius << std::endl;
  std::cout << "world origin:" << data.world.origin().transpose() << std::endl;
  std::cout << "camera radius:" << data.camera_distance << std::endl;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  // glutInitWindowPosition(100, 100);
  glutInitWindowSize(data.rtkernel.cam.pixel_cols(),
                     data.rtkernel.cam.pixel_rows());
  glutCreateWindow("spray: simulated particle ray tracer");

  // glutCloseFunc(close);
  glutDisplayFunc(render);
  glutReshapeFunc(resize);
  // glutIdleFunc(idle);
  glutIdleFunc(render);
  glutKeyboardFunc(process_normal_keys);
  glutSpecialFunc(process_special_keys);
  glutMouseFunc(process_mouse_buttons);
  glutPassiveMotionFunc(process_passive_mouse_move);
  glutMotionFunc(process_mouse_move);
  // glutMouseWheelFunc(process_mouse_wheel);

  // glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
  // GLUT_ACTION_GLUTMAINLOOP_RETURNS);

  glClearColor(1, 0, 0, 1);
}

void exec() { glutMainLoop(); }

void close() {}

void resize(int width, int height) {
  if (height == 0) height = 1;

  data.rtkernel.cam.set_screen_resolution(height, width);

  const float aspect_ratio = static_cast<float>(width) / height;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, width, height);
  gluPerspective(45.0f, aspect_ratio, 0.1f, 1000.0f);
  glMatrixMode(GL_MODELVIEW);
}

void render() {
  data.fpsm.measure();

  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  data.rtkernel.cam.look_at(ray_tracer::horizontal_coordinates(
                                data.world, data.camera_distance,
                                data.camera_altitude, data.camera_azimuth),
                            data.world.center(), data.world.up());

  data.rtkernel.render();
  glDrawPixels(data.rtkernel.cam.pixel_cols(), data.rtkernel.cam.pixel_rows(),
               GL_RGBA, GL_FLOAT, data.rtkernel.accum_buffer.data());

  glutSwapBuffers();
}

void idle() {}

void process_normal_keys(unsigned char key, int x, int y) {
  switch (key) {
    case glut_key_esc:
      exit(0);
      break;
  }
}

void process_special_keys(int key, int x, int y) {
  switch (key) {
    case GLUT_KEY_LEFT:
      data.camera_azimuth += 0.1f;
      break;
    case GLUT_KEY_RIGHT:
      data.camera_azimuth -= 0.1f;
      break;
    case GLUT_KEY_UP:
      data.camera_altitude += 0.1f;
      break;
    case GLUT_KEY_DOWN:
      data.camera_altitude -= 0.1f;
      break;
  }

  glutPostRedisplay();
}

void process_mouse_buttons(int button, int button_state, int x, int y) {
  data.key_modifiers = glutGetModifiers();
}

void process_mouse_move(int x, int y) {
  switch (data.key_modifiers) {
    case GLUT_ACTIVE_CTRL:
      data.camera_distance += static_cast<float>(y - data.old_mouse_y) * 0.1f;
      if (data.camera_distance < 0.0f) data.camera_distance = 0.0f;
      break;

    default:
      data.camera_azimuth -= static_cast<float>(x - data.old_mouse_x) * 0.01f;
      data.camera_altitude -= static_cast<float>(y - data.old_mouse_y) * 0.01f;
      if (data.camera_altitude > M_PI_2 - 0.0001f)
        data.camera_altitude = M_PI_2 - 0.0001f;
      if (data.camera_altitude < -M_PI_2 + 0.0001f)
        data.camera_altitude = -M_PI_2 + 0.0001f;
  }

  data.old_mouse_x = x;
  data.old_mouse_y = y;
}

void process_passive_mouse_move(int x, int y) {
  data.old_mouse_x = x;
  data.old_mouse_y = y;
}

void process_mouse_wheel(int wheel, int direction, int x, int y) {}

}  // namespace glut_app
}  // namespace spray