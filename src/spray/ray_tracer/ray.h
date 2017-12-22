#ifndef SPRAY_RAY_TRACER_RAY_H_
#define SPRAY_RAY_TRACER_RAY_H_

#include <Eigen/Dense>
#include <utility>

namespace spray {
namespace ray_tracer {

class Ray {
 public:
  Ray() = default;
  Ray(const Ray&) = default;
  Ray& operator=(const Ray&) = default;
  Ray(Ray&&) = default;
  Ray& operator=(Ray&&) = default;
  ~Ray() = default;

  Ray(const Eigen::Vector3f& origin, const Eigen::Vector3f& direction)
      : origin_(origin), direction_(direction) {}

  Ray(const std::pair<Eigen::Vector3f, Eigen::Vector3f>& point_pair)
      : origin_(point_pair.first),
        direction_(point_pair.second - point_pair.first) {}

  Eigen::Vector3f operator()(float t) const { return origin_ + t * direction_; }

  Eigen::Vector3f& origin() { return origin_; }
  const Eigen::Vector3f& origin() const { return origin_; }
  Eigen::Vector3f& direction() { return direction_; }
  const Eigen::Vector3f& direction() const { return direction_; }

 private:
  Eigen::Vector3f origin_;
  Eigen::Vector3f direction_;
};

class Cached_ray : public Ray {
 public:
  explicit Cached_ray(const Ray& r)
      : Ray(r),
        inverse_direction_(r.direction().array().inverse().matrix()),
        direction_is_negative_(Eigen::Vector3i{r.direction()(0) < 0.0f,
                                               r.direction()(1) < 0.0f,
                                               r.direction()(2) < 0.0f}) {}

  const Eigen::Vector3f& inverse_direction() const {
    return inverse_direction_;
  }
  int direction_is_negative(int i) const { return direction_is_negative_[i]; }

 private:
  Eigen::Vector3f inverse_direction_;
  Eigen::Vector3i direction_is_negative_;
};

}  // namespace ray_tracer
}  // namespace spray

#endif  // SPRAY_RAY_TRACER_RAY_H_