#ifndef SPRAY_ORTHONORMAL_FRAME_H_
#define SPRAY_ORTHONORMAL_FRAME_H_

#include <Eigen/Dense>
#include <cmath>

namespace spray {
namespace ray_tracer {

class orthonormal_frame {
  using Vector3f = Eigen::Vector3f;

 public:
  orthonormal_frame(const Vector3f& center = Vector3f(0.0f, 0.0f, 0.0f),
                    const Vector3f& up = Vector3f(0.0f, 1.0f, 0.0f),
                    const Vector3f& right = Vector3f(1.0f, 0.0f, 0.0f))
      : center_(center), up_(up), right_(right) {
    compute_basis();
  }

  Vector3f center() const { return center_; }
  Vector3f origin() const { return center_; }
  Vector3f up() const { return up_; }
  Vector3f right() const { return right_; }
  Vector3f back() const { return back_; }

  void set_center(const Vector3f& center) { center_ = center; }
  void set_origin(const Vector3f& origin) { center_ = origin; }
  void set_basis(const Vector3f& up, const Vector3f& right);

 private:
  void compute_basis();

 private:
  Vector3f center_;
  Vector3f up_;
  Vector3f right_;
  Vector3f back_;
};

Eigen::Vector3f horizontal_coordinates(const orthonormal_frame& base_system,
                                       float radius, float altitude,
                                       float azimuth);

}  // namespace ray_tracer
}  // namespace spray

#endif  // SPRAY_ORTHONORMAL_FRAME_H_