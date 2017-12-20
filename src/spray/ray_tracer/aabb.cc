#include "aabb.h"

namespace spray {
namespace ray_tracer {

aabb bounds(const Eigen::Vector3f& v1, const Eigen::Vector3f& v2) {
  const Eigen::Vector3f min = static_cast<Eigen::Vector3f>(
      static_cast<Eigen::Array3f>(v1).min(static_cast<Eigen::Array3f>(v2)));
  const Eigen::Vector3f max = static_cast<Eigen::Vector3f>(
      static_cast<Eigen::Array3f>(v2).max(static_cast<Eigen::Array3f>(v1)));
  return aabb{min, max};
}

aabb bounds(const aabb& box1, const aabb& box2) {
  const Eigen::Vector3f min =
      static_cast<Eigen::Vector3f>(static_cast<Eigen::Array3f>(box1.min).min(
          static_cast<Eigen::Array3f>(box2.min)));
  const Eigen::Vector3f max =
      static_cast<Eigen::Vector3f>(static_cast<Eigen::Array3f>(box1.max).max(
          static_cast<Eigen::Array3f>(box2.max)));
  return aabb{min, max};
}

aabb bounds(const aabb& box, const Eigen::Vector3f& v) {
  const Eigen::Vector3f min = static_cast<Eigen::Vector3f>(
      static_cast<Eigen::Array3f>(box.min).min(static_cast<Eigen::Array3f>(v)));
  const Eigen::Vector3f max = static_cast<Eigen::Vector3f>(
      static_cast<Eigen::Array3f>(box.max).max(static_cast<Eigen::Array3f>(v)));
  return aabb{min, max};
}

Eigen::Vector3f center(const aabb& box) { return 0.5f * (box.min + box.max); }

float radius(const aabb& box) { return 0.5f * (box.max - box.min).norm(); }

}  // namespace ray_tracer
}  // spray