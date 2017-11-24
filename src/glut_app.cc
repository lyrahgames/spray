#include "glut_app.h"

namespace spray {
namespace glut_app {

state_t state{};

void init(int argc, char** argv) {
  state.angle = 0.0f;
  state.cube_size = 2.0f;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(320, 320);
  glutCreateWindow("GLUT Tutorial");

  glutCloseFunc(close);
  glutDisplayFunc(render);
  glutReshapeFunc(resize);
  glutIdleFunc(idle);
  glutKeyboardFunc(process_normal_keys);
  glutSpecialFunc(process_special_keys);
  glutMouseFunc(process_mouse_buttons);
  glutMotionFunc(process_mouse_move);
  glutMouseWheelFunc(process_mouse_wheel);

  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
}

void exec() { glutMainLoop(); }

void close() { std::cout << "succesful..." << std::flush; }

void resize(int width, int height) {
  if (height == 0) height = 1;

  const float aspect_ratio = static_cast<float>(width) / height;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, width, height);
  gluPerspective(45.0f, aspect_ratio, 0.1f, 100.0f);
  glMatrixMode(GL_MODELVIEW);
}

void render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();
  gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
  glRotatef(state.angle, 0.0f, 1.0f, 0.0f);

  glBegin(GL_TRIANGLES);
  glVertex3f(-2.0f, -2.0f, 0.0f);
  glVertex3f(2.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, 2.0f, 0.0f);
  glEnd();

  glutWireCube(state.cube_size);

  glutSwapBuffers();
}

void idle() {}

void process_normal_keys(unsigned char key, int x, int y) {
  switch (key) {
    case glut_key_esc:
      glutLeaveMainLoop();
      break;
  }
}

void process_special_keys(int key, int x, int y) {
  switch (key) {
    case GLUT_KEY_LEFT:
      state.angle += 1.0f;
      break;
    case GLUT_KEY_RIGHT:
      state.angle -= 1.0f;
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