#ifndef SPRAY_RAY_TRACER_BOUNDING_BOX_H_
#define SPRAY_RAY_TRACER_BOUNDING_BOX_H_

#include <Eigen/Dense>
#include <cassert>
#include <vector>

namespace spray {
namespace ray_tracer {

class Bounding_box;

Eigen::Vector3f center(const Bounding_box& box);
float radius(const Bounding_box& box);

class Bounding_box {
 public:
  Bounding_box() = default;
  Bounding_box(const Bounding_box&) = default;
  Bounding_box& operator=(const Bounding_box&) = default;
  Bounding_box(Bounding_box&&) = default;
  Bounding_box& operator=(Bounding_box&&) = default;
  ~Bounding_box() = default;

  Bounding_box(const Eigen::Vector3f& min, const Eigen::Vector3f& max)
      : min_(min.array().min(max.array()).matrix()),
        max_(max.array().max(min.array()).matrix()) {}
  Bounding_box(const Eigen::Vector3f& point) : min_(point), max_(point) {}
  Bounding_box(const Bounding_box& box1, const Bounding_box& box2)
      : min_(box1.min().array().min(box2.min().array()).matrix()),
        max_(box1.max().array().max(box2.max().array()).matrix()) {}
  Bounding_box(const Bounding_box& box, const Eigen::Vector3f& point)
      : min_(box.min().array().min(point.array()).matrix()),
        max_(box.max().array().max(point.array()).matrix()) {}

  const Eigen::Vector3f& min() const { return min_; }
  const Eigen::Vector3f& max() const { return max_; }
  const Eigen::Vector3f& operator[](int index) const { return (&min_)[index]; }

  float radius() { return ray_tracer::radius(*this); }
  Eigen::Vector3f center() { return ray_tracer::center(*this); }

 private:
  Eigen::Vector3f min_;
  Eigen::Vector3f max_;
};

}  // namespace ray_tracer
}  // namespace spray

#endif  // SPRAY_RAY_TRACER_BOUNDING_BOX_H_