#include "orthonormal_frame.h"

namespace spray {
namespace ray_tracer {

orthonormal_frame::orthonormal_frame()
    : origin_(Vector3f(0.0f, 0.0f, 0.0f)),
      back_(Vector3f(0.0f, 0.0f, 1.0f)),
      up_(Vector3f(0.0f, 1.0f, 0.0f)),
      right_(Vector3f(1.0f, 0.0f, 0.0f)) {}

orthonormal_frame::orthonormal_frame(const Vector3f& origin,
                                     const Vector3f& back, const Vector3f& up)
    : origin_(origin), back_(back), up_(up) {
  compute_basis_from_back_and_up();
}

void orthonormal_frame::compute_basis_from_up_and_back() {
  up_.normalize();
  right_ = up_.cross(back_);
  right_.normalize();
  back_ = right_.cross(up_);
  back_.normalize();
}

void orthonormal_frame::compute_basis_from_back_and_up() {
  back_.normalize();
  right_ = up_.cross(back_);
  right_.normalize();
  up_ = back_.cross(right_);
  up_.normalize();
}

Eigen::Vector3f horizontal_coordinates(const orthonormal_frame& system,
                                       float radius, float altitude,
                                       float azimuth) {
  const float x = radius * cosf(azimuth) * cosf(altitude);
  const float y = radius * sinf(azimuth) * cosf(altitude);
  const float z = radius * sinf(altitude);
  return y * system.right() + z * system.up() + x * system.back() +
         system.origin();
}

}  // namespace ray_tracer
}  // namespace spray