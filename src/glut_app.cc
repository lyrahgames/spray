#include "glut_app.h"

namespace spray {
namespace glut_app {

state data{};

void init(int argc, char** argv) {
  data.angle = 0.0f;
  data.cube_size = 2.0f;
  data.distance = 100.0f;

  if (argc != 2) {
    std::cout << "usage: spray <file name>" << std::endl;
    exit(0);
  }

  data.rtkernel.s = ray_tracer::load_stl(argv[1]);

  data.rtkernel.cam.set_screen_resolution(320, 320);
  data.rtkernel.cam.set_field_of_view(0.5f * M_PI);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(data.rtkernel.cam.pixel_cols(),
                     data.rtkernel.cam.pixel_rows());
  glutCreateWindow("spray: simulated particle ray tracer");

  // glutCloseFunc(close);
  glutDisplayFunc(render);
  glutReshapeFunc(resize);
  glutIdleFunc(idle);
  glutKeyboardFunc(process_normal_keys);
  glutSpecialFunc(process_special_keys);
  glutMouseFunc(process_mouse_buttons);
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
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // glLoadIdentity();
  // gluLookAt(0.0f, 0.0f, data.distance, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
  // glRotatef(data.angle, 0.0f, 1.0f, 0.0f);

  // glBegin(GL_TRIANGLES);
  // glVertex3f(-2.0f, -2.0f, 0.0f);
  // glVertex3f(2.0f, 0.0f, 0.0f);
  // glVertex3f(0.0f, 2.0f, 0.0f);
  // glEnd();

  // glBegin(GL_TRIANGLES);
  // for (std::size_t i = 0; i < data.rtkernel.s.primitive_vector.size(); ++i) {
  //   glVertex3fv(static_cast<GLfloat*>(
  //       data.rtkernel.s.primitive_vector[i].vertex[0].data()));
  //   glVertex3fv(static_cast<GLfloat*>(
  //       data.rtkernel.s.primitive_vector[i].vertex[1].data()));
  //   glVertex3fv(static_cast<GLfloat*>(
  //       data.rtkernel.s.primitive_vector[i].vertex[2].data()));
  // }
  // glEnd();

  // glutWireCube(data.cube_size);

  data.rtkernel.cam.look_at(
      data.distance * Eigen::Vector3f(cosf(data.angle), 0, sinf(data.angle)),
      Eigen::Vector3f(0, 0, 0), Eigen::Vector3f(0, 1, 0));
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
      data.angle += 0.1f;
      break;
    case GLUT_KEY_RIGHT:
      data.angle -= 0.1f;
      break;
    case GLUT_KEY_UP:
      data.distance += 0.5f;
      break;
    case GLUT_KEY_DOWN:
      data.distance -= 0.5f;
      break;
  }

  glutPostRedisplay();
}

void process_mouse_buttons(int button, int button_state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON) {
    if (button_state == GLUT_DOWN) {
    }
  }
}

void process_mouse_move(int x, int y) {}

void process_mouse_wheel(int wheel, int direction, int x, int y) {}

}  // namespace glut_app
}  // namespace spray