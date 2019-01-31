#ifndef SPRAY_VIEWER_VIEWER_H_
#define SPRAY_VIEWER_VIEWER_H_

#include <string>

#include <QApplication>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QWheelEvent>

#include <spray/spray.h>

#include "camera.h"
#include "isometry.h"

class Viewer : public QOpenGLWidget {
  Q_OBJECT

 public:
  Viewer(const std::string& file_path, QWidget* parent = nullptr);

 protected:
  void initializeGL() override;
  void resizeGL(int width, int height) override;
  void paintGL() override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;

 private:
  int old_mouse_x = 0;
  int old_mouse_y = 0;

  Isometry world_{{}, {0, -1, 0}, {0, 0, 1}};
  Camera camera_{};
  float eye_distance = 10.0f;
  float eye_azimuth = 0.0f;
  float eye_altitude = 0.0f;

  spray::Scene scene_;
  spray::Bounding_box bounding_box_;

  void compute_look_at();
  void compute_automatic_view();
};

#endif  // SPRAY_VIEWER_VIEWER_H_