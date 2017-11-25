#ifndef SPRAY_INTERSECTION_H_
#define SPRAY_INTERSECTION_H_

#include "primitive.h"
#include "ray.h"

namespace spray {
namespace ray_tracer {

constexpr float intersection_epsilon = 1e-6;

struct intersection {
  float u, v, t;
};

// moeller-trumbore-algorithm
bool intersect(const ray& r, const primitive& p);

}  // namespace ray_tracer
}  // namespace spray

#endif  // SPRAY_INTERSECTION_H_