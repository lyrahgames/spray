#ifndef SPRAY_AABB_H_
#define SPRAY_AABB_H_

#include <Eigen/Dense>

namespace spray {
namespace ray_tracer {

struct aabb {
  Eigen::Vector3f min;
  Eigen::Vector3f max;
};

aabb bounds(const Eigen::Vector3f& v1, const Eigen::Vector3f& v2);
aabb bounds(const aabb& box1, const aabb& box2);
aabb bounds(const aabb& box, const Eigen::Vector3f& v);
Eigen::Vector3f center(const aabb& box);
float radius(const aabb& box);

}  // namespace ray_tracer
}  // namespace spray

#endif  // SPRAY_AABB_H_