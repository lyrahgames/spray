#include "camera.h"

namespace spray {
namespace ray_tracer {

float camera::vertical_field_of_view() const {
  // return 2.0 * atanf(tanf(field_of_view_ * 0.5f) / aspect_ratio_);
  return field_of_view_;
}

float camera::horizontal_field_of_view() const {
  return 2.0 * std::atan(std::tan(field_of_view_ * 0.5f) * aspect_ratio_);
}

float camera::opengl_field_of_view() const {
  return field_of_view_ * 180 / M_PI;
}

void camera::look_at(const Eigen::Vector3f& eye, const Eigen::Vector3f& center,
                     const Eigen::Vector3f& up) {
  frame_ = orthonormal_frame(eye, eye - center, up);
}

void camera::set_screen_resolution(int width, int height) {
  screen_height_ = height;
  screen_width_ = width;
  compute_pixel_size();
  compute_aspect_ratio();
}

void camera::set_field_of_view(float fov) {
  if (fov <= 0.0f || fov >= static_cast<float>(M_PI)) return;
  field_of_view_ = fov;
  compute_pixel_size();
}

void camera::compute_pixel_size() {
  // pixel_size_ =
  //     2.0f * tanf(field_of_view_ * 0.5f) / static_cast<float>(screen_width_);
  pixel_size_ = 2.0f * std::tan(field_of_view_ * 0.5f) /
                static_cast<float>(screen_height_);
}

void camera::compute_aspect_ratio() {
  aspect_ratio_ =
      static_cast<float>(screen_width_) / static_cast<float>(screen_height_);
}

ray primary_ray(const camera& cam, int col, int row) {
  ray r{cam.position(),
        cam.direction() +
            cam.pixel_size() *
                ((static_cast<float>(col) -
                  0.5f * static_cast<float>(cam.screen_width()) + 0.5f) *
                     cam.frame().right() +
                 (static_cast<float>(row) -
                  0.5f * static_cast<float>(cam.screen_height()) + 0.5f) *
                     cam.frame().up())};

  r.direction.normalize();
  return r;
}

}  // namespace ray_tracer
}  // namespace spray