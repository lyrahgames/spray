#ifndef SPRAY_AABB_H_
#define SPRAY_AABB_H_

#include <Eigen/Dense>
#include <iostream>

namespace spray {
namespace ray_tracer {

class aabb {
 public:
  aabb(const Eigen::Vector3f& min, const Eigen::Vector3f& max) {
    min_ = static_cast<Eigen::Vector3f>(
        static_cast<Eigen::Array3f>(min).min(static_cast<Eigen::Array3f>(max)));
    max_ = static_cast<Eigen::Vector3f>(
        static_cast<Eigen::Array3f>(max).max(static_cast<Eigen::Array3f>(min)));
  }

  const Eigen::Vector3f& min() const { return min_; }
  const Eigen::Vector3f& max() const { return max_; }

 private:
  Eigen::Vector3f min_;
  Eigen::Vector3f max_;
};

}  // namespace ray_tracer
}  // namespace spray

inline std::ostream& operator<<(std::ostream& os,
                                const spray::ray_tracer::aabb& box) {
  return os << "[(" << box.min().transpose() << "), (" << box.max().transpose()
            << ")]";
}

#endif  // SPRAY_AABB_H_