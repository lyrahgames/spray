#ifndef SPRAY_VIEWER_CAMERA_H_
#define SPRAY_VIEWER_CAMERA_H_

#include <Eigen/Dense>

#include "isometry.h"

class Camera {
  using Vector3f = Eigen::Vector3f;

 public:
  Camera() = default;

  Vector3f position() const { return frame_.origin(); }
  Vector3f direction() const { return -frame_.basis_z(); }
  Vector3f right() const { return frame_.basis_x(); }
  Vector3f up() const { return frame_.basis_y(); }

  Camera& frame(const Isometry& frame);
  Isometry& frame() { return frame_; }
  const Isometry& frame() const { return frame_; }

  float field_of_view() const { return vertical_field_of_view_; }
  float vertical_field_of_view() const { return vertical_field_of_view_; }
  float horizontal_field_of_view() const;
  float opengl_field_of_view() const;
  float near_distance() const { return near_distance_; }
  float far_distance() const { return far_distance_; }
  int screen_width() const { return screen_width_; }
  int screen_height() const { return screen_height_; }
  float pixel_size() const;
  float aspect_ratio() const;

  Eigen::Matrix4f view_matrix() const;
  Eigen::Matrix4f projection_matrix() const;

  Camera& look_at(const Vector3f& eye, const Vector3f& center,
                  const Vector3f& up);
  Camera& perspective(float fov, float near, float far);
  Camera& screen_resolution(int width, int height);
  Camera& field_of_view(float fov);
  Camera& vertical_field_of_view(float fov);
  Camera& horizontal_field_of_view(float fov);
  Camera& near_and_far_distance(float near, float far);

 private:
  Isometry frame_{};
  float vertical_field_of_view_{M_PI / 4.0f};
  float near_distance_{0.1f};
  float far_distance_{1000.0f};
  int screen_height_{640};
  int screen_width_{360};
};

#endif  // SPRAY_VIEWER_CAMERA_H_