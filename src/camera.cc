#include "camera.h"

namespace spray {
namespace ray_tracer {

void camera::look_at(const Eigen::Vector3f& eye, const Eigen::Vector3f& center,
                     const Eigen::Vector3f& up) {
  position_ = eye;
  direction_ = center - eye;
  direction_.normalize();
  right_ = direction_.cross(up);
  right_.normalize();
  up_ = right_.cross(direction_);
  up_.normalize();
}

void camera::set_screen_resolution(int width, int height) {
  pixel_rows_ = width;
  pixel_cols_ = height;
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
      2.0f * tanf(field_of_view_ * 0.5f) / static_cast<float>(pixel_cols_);
}

void camera::compute_aspect_ratio() {
  aspect_ratio_ =
      static_cast<float>(pixel_cols_) / static_cast<float>(pixel_rows_);
}

ray camera::primary_ray(int col, int row) const {
  return ray{
      position_,
      direction_ +
          static_cast<float>(col - pixel_cols_ / 2) * pixel_size_ * right_ -
          static_cast<float>(row - pixel_rows_ / 2) * pixel_size_ * up_};
}

}  // namespace ray_tracer
}  // namespace spray