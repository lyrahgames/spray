#include "orthonormal_frame.h"

namespace spray {
namespace ray_tracer {

void orthonormal_frame::set_basis(const Eigen::Vector3f& up,
                                  const Eigen::Vector3f& right) {
  up_ = up;
  right_ = right;
  compute_basis();
}

void orthonormal_frame::compute_basis() {
  up_.normalize();
  back_ = right_.cross(up_);
  back_.normalize();
  right_ = up_.cross(back_);
  right_.normalize();
}

Eigen::Vector3f horizontal_coordinates(const orthonormal_frame& system,
                                       float radius, float altitude,
                                       float azimuth) {
  const float x = radius * cosf(azimuth) * cosf(altitude);
  const float y = radius * sinf(azimuth) * cosf(altitude);
  const float z = radius * sinf(altitude);
  return y * system.right() + z * system.up() + x * system.back();
}

}  // namespace ray_tracer
}  // namespace spray