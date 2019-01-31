#include "camera.h"
#include <cmath>
#include <stdexcept>

Camera& Camera::frame(const Isometry& frame) {
  frame_ = frame;
  return *this;
}

float Camera::horizontal_field_of_view() const {
  return 2.0 *
         std::atan(std::tan(vertical_field_of_view_ * 0.5f) * aspect_ratio());
}

float Camera::opengl_field_of_view() const {
  // unit has to be degree
  return vertical_field_of_view_ * 180 / M_PI;
}

float Camera::pixel_size() const {
  return 2.0f * std::tan(vertical_field_of_view_ * 0.5f) /
         static_cast<float>(screen_height_);
}

float Camera::aspect_ratio() const {
  return static_cast<float>(screen_width_) / static_cast<float>(screen_height_);
}

Eigen::Matrix4f Camera::view_matrix() const {
  Eigen::Matrix3f rotation;
  for (int i = 0; i < 3; ++i) rotation.row(i) = frame_.matrix().col(i);
  Eigen::Vector3f translation = -rotation * frame_.matrix().col(4);

  Eigen::Matrix4f matrix = Eigen::Matrix4f::Identity();
  matrix.block<3, 3>(0, 0) = rotation;
  matrix.block<3, 1>(0, 3) = translation;
  // for (int i = 0; i < 3; ++i) matrix.row(i) = frame_.matrix().row(i);
  return matrix;
}

Eigen::Matrix4f Camera::projection_matrix() const {
  Eigen::Matrix4f matrix;
  const float f = 1.0f / (std::tan(0.5f * vertical_field_of_view_));
  matrix << f / aspect_ratio(), 0, 0, 0, 0, f, 0, 0, 0, 0,
      (far_distance_ + near_distance_) / (near_distance_ - far_distance_),
      2 * near_distance_ * far_distance_ / (near_distance_ - far_distance_), 0,
      0, -1, 0;
  return matrix;
}

Camera& Camera::look_at(const Eigen::Vector3f& eye,
                        const Eigen::Vector3f& center,
                        const Eigen::Vector3f& up) {
  frame_ =
      Isometry{eye, eye - center, up, Isometry::Positive_zy_construction{}};
  return *this;
}

Camera& Camera::perspective(float fov, float near, float far) {
  vertical_field_of_view(fov);
  near_and_far_distance(near, far);
  return *this;
}

Camera& Camera::screen_resolution(int width, int height) {
  if (width <= 0 || height <= 0)
    throw std::invalid_argument(
        "Width and height of the camera screen have to be bigger than zero.");
  screen_height_ = height;
  screen_width_ = width;
  return *this;
}

Camera& Camera::field_of_view(float fov) {
  if (fov <= 0.0f || fov >= static_cast<float>(M_PI))
    throw std::invalid_argument(
        "Camera field of view has to be in the range (0,π/2)!");
  vertical_field_of_view_ = fov;
  return *this;
}

Camera& Camera::vertical_field_of_view(float fov) { return field_of_view(fov); }

Camera& Camera::horizontal_field_of_view(float fov) {
  return field_of_view(2.0 * std::atan(std::tan(fov * 0.5f) / aspect_ratio()));
}

Camera& Camera::near_and_far_distance(float near, float far) {
  if (near <= 0.0f || far <= 0.0f || near >= far)
    throw std::invalid_argument("Camera near and far distances are invalid!");
  near_distance_ = near;
  far_distance_ = far;
  return *this;
}