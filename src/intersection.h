#ifndef SPRAY_INTERSECTION_H_
#define SPRAY_INTERSECTION_H_

#include <Eigen/Dense>
#include "primitive.h"
#include "ray.h"

namespace spray {
namespace ray_tracer {

constexpr float intersection_epsilon = 1e-6;

// moeller-trumbore-algorithm
bool intersect(const ray& r, const Eigen::Vector3f& vertex0,
               const Eigen::Vector3f& vertex1, const Eigen::Vector3f& vertex2,
               Eigen::Vector3f& uvt);

}  // namespace ray_tracer
}  // namespace spray

#endif  // SPRAY_INTERSECTION_H_