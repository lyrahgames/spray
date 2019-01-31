#include "viewer.h"

#include <cmath>
#include <iostream>

#include <GL/glu.h>

using namespace std;

Viewer::Viewer(const string& file_path, QWidget* parent)
    : QOpenGLWidget(parent), scene_{file_path}, bounding_box_{bounds(scene_)} {
  setMouseTracking(true);
  compute_automatic_view();
  compute_look_at();
  show();
}

void Viewer::initializeGL() {
  // glEnable(GL_DEPTH_TEST);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glClearColor(0, 0, 0, 1.0f);
}

void Viewer::resizeGL(int width, int height) {
  camera_.screen_resolution(width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, camera_.screen_width(), camera_.screen_height());
  gluPerspective(camera_.opengl_field_of_view(), camera_.aspect_ratio(), 0.1f,
                 1000.0f);
  glMatrixMode(GL_MODELVIEW);

  compute_look_at();
}

void Viewer::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (int i = 0; i < static_cast<int>(scene_.primitive_data().size()); ++i) {
    const auto& p = scene_.primitive_data()[i];

    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3fv(reinterpret_cast<const GLfloat*>(
        scene_.vertex_data()[p.vertex_id[0]].position.data()));
    glVertex3fv(reinterpret_cast<const GLfloat*>(
        scene_.vertex_data()[p.vertex_id[1]].position.data()));
    glVertex3fv(reinterpret_cast<const GLfloat*>(
        scene_.vertex_data()[p.vertex_id[2]].position.data()));
    glEnd();
  }
}

void Viewer::mousePressEvent(QMouseEvent* event) {}

void Viewer::mouseReleaseEvent(QMouseEvent* event) {}

void Viewer::mouseMoveEvent(QMouseEvent* event) {
  const int x_difference = event->x() - old_mouse_x;
  const int y_difference = event->y() - old_mouse_y;

  if (event->buttons() == Qt::LeftButton) {
    constexpr float max_altitude = M_PI_2 - 0.0001f;
    constexpr float scale_factor = 0.01;

    eye_azimuth += x_difference * scale_factor;
    eye_altitude += y_difference * scale_factor;

    if (eye_altitude > max_altitude)
      eye_altitude = max_altitude;
    else if (eye_altitude < -max_altitude)
      eye_altitude = -max_altitude;

  } else if (event->buttons() == Qt::RightButton) {
    world_.translation() =
        world_.origin() -
        camera_.right() * x_difference * camera_.pixel_size() * eye_distance +
        camera_.up() * y_difference * camera_.pixel_size() * eye_distance;
  }

  compute_look_at();

  old_mouse_x = event->x();
  old_mouse_y = event->y();
}

void Viewer::wheelEvent(QWheelEvent* event) {
  constexpr float scale_factor = -0.003;
  constexpr float min_distance = 1e-5f;

  eye_distance += scale_factor * event->angleDelta().y() * eye_distance;
  if (eye_distance < min_distance) eye_distance = min_distance;

  compute_look_at();
}

void Viewer::keyPressEvent(QKeyEvent* event) {
  if (event->key() == Qt::Key_Escape) {
    QApplication::quit();
  } else if (event->text() == "b") {
    eye_azimuth = eye_altitude = 0.0f;
    world_ = Isometry{0.5f * (bounding_box_.min() + bounding_box_.max()),
                      {0, -1, 0},
                      {0, 0, 1}};
  } else if (event->text() == "g") {
    eye_azimuth = eye_altitude = 0.0f;
    world_ = Isometry{0.5f * (bounding_box_.min() + bounding_box_.max()),
                      {0, 0, 1},
                      {0, 1, 0}};
  }

  compute_look_at();
}

void Viewer::keyReleaseEvent(QKeyEvent* event) {}

void Viewer::compute_look_at() {
  Eigen::Vector3f position;
  position[0] = eye_distance * cos(eye_azimuth) * cos(eye_altitude);
  position[1] = eye_distance * sin(eye_altitude);
  position[2] = eye_distance * sin(eye_azimuth) * cos(eye_altitude);
  position = world_ * position;

  camera_.look_at(position, world_.origin(), world_.basis_y())
      .field_of_view(M_PI_4)
      .near_and_far_distance(0.001f, 10000.0f);

  makeCurrent();
  glLoadIdentity();
  gluLookAt(camera_.position()(0), camera_.position()(1), camera_.position()(2),
            world_.origin()(0), world_.origin()(1), world_.origin()(2),
            world_.basis_y()(0), world_.basis_y()(1), world_.basis_y()(2));

  update();
}

void Viewer::compute_automatic_view() {
  const float scene_radius =
      0.5f * (bounding_box_.max() - bounding_box_.min()).norm();
  eye_distance = scene_radius / sin(0.5f * camera_.field_of_view());
  world_.translation() = 0.5f * (bounding_box_.min() + bounding_box_.max());
  update();
}