#include "glut_app.h"

namespace spray {
namespace glut_app {

state_t state{};

void init(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(320, 320);
  glutCreateWindow("GLUT Tutorial");

  glutCloseFunc(close);
  glutDisplayFunc(render);
  glutReshapeFunc(resize);
  glutIdleFunc(render);
  glutKeyboardFunc(process_normal_keys);
  glutSpecialFunc(process_special_keys);

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

  glutSwapBuffers();

  state.angle += 0.3f;
}

void process_normal_keys(unsigned char key, int x, int y) {
  switch (key) {
    case glut_key_esc:
      glutLeaveMainLoop();
      break;
  }
}

void process_special_keys(int key, int x, int y) {
  switch (key) {
    case GLUT_KEY_UP:
      glutLeaveMainLoop();
      break;
  }
}

}  // namespace glut_app
}  // namespace spray