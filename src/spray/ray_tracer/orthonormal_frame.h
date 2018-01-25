#ifndef SPRAY_RAY_TRACER_ORTHONORMAL_FRAME_H_
#define SPRAY_RAY_TRACER_ORTHONORMAL_FRAME_H_

#include <Eigen/Dense>
#include <cmath>

namespace spray {
namespace ray_tracer {

class Orthonormal_frame {
  using Vector3f = Eigen::Vector3f;

 public:
  Orthonormal_frame();
  Orthonormal_frame(const Vector3f& origin, const Vector3f& up,
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

inline Orthonormal_frame blender_orthonormal_frame(
    const Eigen::Vector3f& origin) {
  return Orthonormal_frame(origin, Eigen::Vector3f(0.0, -1.0f, 0.0f),
                           Eigen::Vector3f(0.0f, 0.0f, 1.0f));
}

inline Orthonormal_frame opengl_orthonormal_frame(
    const Eigen::Vector3f& origin) {
  return Orthonormal_frame(origin, Eigen::Vector3f(0.0f, 0.0f, 1.0f),
                           Eigen::Vector3f(0.0f, 1.0f, 0.0f));
}

Eigen::Vector3f horizontal_coordinates(const Orthonormal_frame& base_system,
                                       float radius, float altitude,
                                       float azimuth);

}  // namespace ray_tracer
}  // namespace spray

#endif  // SPRAY_RAY_TRACER_ORTHONORMAL_FRAME_H_