#include "camera.h"

namespace spray {
namespace ray_tracer {

float camera::vertical_field_of_view() const {
  return 2.0 * atanf(tanf(field_of_view_ * 0.5f) / aspect_ratio_);
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
  pixel_size_ =
      2.0f * tanf(field_of_view_ * 0.5f) / static_cast<float>(screen_width_);
}

void camera::compute_aspect_ratio() {
  aspect_ratio_ =
      static_cast<float>(screen_width_) / static_cast<float>(screen_height_);
}

ray primary_ray(const camera& cam, int col, int row) {
  return ray{cam.position(),
             cam.direction() +
                 static_cast<float>(col - cam.screen_width() / 2) *
                     cam.pixel_size() * cam.frame().right() +
                 static_cast<float>(row - cam.screen_height() / 2) *
                     cam.pixel_size() * cam.frame().up()};
}

}  // namespace ray_tracer
}  // namespace spray