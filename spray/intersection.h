#ifndef SPRAY_RAY_TRACER_INTERSECTION_H_
#define SPRAY_RAY_TRACER_INTERSECTION_H_

#include <Eigen/Dense>
#include "bounding_box.h"
#include "ray.h"

namespace spray {

constexpr float intersection_epsilon = 1e-7;

// moeller-trumbore-algorithm
bool intersect(const Ray& r, const Eigen::Vector3f& vertex0,
               const Eigen::Vector3f& vertex1, const Eigen::Vector3f& vertex2,
               Eigen::Vector3f& uvt);

bool intersect(const Ray& r, const Bounding_box& box);
bool intersect(const Ray& r, const Bounding_box& box, float& t);

bool intersect(const Cached_ray& r, const Bounding_box& box);
bool intersect(const Cached_ray& r, const Bounding_box& box, float* t);

}  // namespace spray

#endif  // SPRAY_RAY_TRACER_INTERSECTION_H_