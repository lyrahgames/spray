#ifndef SPRAY_INTERSECTION_H_
#define SPRAY_INTERSECTION_H_

#include <Eigen/Dense>
#include "aabb.h"
#include "ray.h"

namespace spray {
namespace ray_tracer {

constexpr float intersection_epsilon = 1e-7;

// moeller-trumbore-algorithm
bool intersect(const ray& r, const Eigen::Vector3f& vertex0,
               const Eigen::Vector3f& vertex1, const Eigen::Vector3f& vertex2,
               Eigen::Vector3f& uvt);

bool intersect(const ray& r, const aabb& box);
bool intersect(const ray& r, const aabb& box, float& t);

bool intersect(const cached_ray& r, const aabb& box);
bool intersect(const cached_ray& r, const aabb& box, float* t);

}  // namespace ray_tracer
}  // namespace spray

#endif  // SPRAY_INTERSECTION_H_