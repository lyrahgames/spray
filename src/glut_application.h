#ifndef SPRAY_VIEWER_GLUT_APP_H_
#define SPRAY_VIEWER_GLUT_APP_H_

class Glut_application {
 public:
  Glut_application();
  Glut_application(int argc, char** argv);
  ~Glut_application();
  Glut_application(const Glut_application&) = delete;
  Glut_application& operator=(const Glut_application&) = delete;
  Glut_application(Glut_application&&) = delete;
  Glut_application&& operator=(Glut_application&&) = delete;

  void execute();

 private:
  static Glut_application* instance_;
};

#endif  // SPRAY_VIEWER_GLUT_APP_H_