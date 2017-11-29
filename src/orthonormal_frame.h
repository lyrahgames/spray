#ifndef SPRAY_ORTHONORMAL_FRAME_H_
#define SPRAY_ORTHONORMAL_FRAME_H_

#include <Eigen/Dense>
#include <cmath>

namespace spray {
namespace ray_tracer {

class orthonormal_frame {
  using Vector3f = Eigen::Vector3f;

 public:
  orthonormal_frame();
  orthonormal_frame(const Vector3f& origin, const Vector3f& up,
                    const Vector3f& back);

  const Vector3f& origin() const { return origin_; }
  const Vector3f& up() const { return up_; }
  const Vector3f& right() const { return right_; }
  const Vector3f& back() const { return back_; }

  void set_origin(const Vector3f& origin) { origin_ = origin; }

 private:
  void compute_basis_from_up_and_back();
  void compute_basis_from_back_and_up();

 private:
  Vector3f origin_;
  Vector3f back_;
  Vector3f up_;
  Vector3f right_;
};

inline orthonormal_frame blender_orthonormal_frame(
    const Eigen::Vector3f& origin) {
  return orthonormal_frame(origin, Eigen::Vector3f(0.0, -1.0f, 0.0f),
                           Eigen::Vector3f(0.0f, 0.0f, 1.0f));
}

inline orthonormal_frame opengl_orthonormal_frame(
    const Eigen::Vector3f& origin) {
  return orthonormal_frame(origin, Eigen::Vector3f(0.0f, 0.0f, 1.0f),
                           Eigen::Vector3f(0.0f, 1.0f, 0.0f));
}

Eigen::Vector3f horizontal_coordinates(const orthonormal_frame& base_system,
                                       float radius, float altitude,
                                       float azimuth);

}  // namespace ray_tracer
}  // namespace spray

#endif  // SPRAY_ORTHONORMAL_FRAME_H_