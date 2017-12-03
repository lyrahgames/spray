#ifndef SPRAY_RAY_H_
#define SPRAY_RAY_H_

#include <Eigen/Core>

namespace spray {
namespace ray_tracer {

struct ray {
  Eigen::Vector3f origin;
  Eigen::Vector3f direction;

  Eigen::Vector3f operator()(float t) { return origin + t * direction; }
};

}  // namespace ray_tracer
}  // namespace spray

#endif  // SPRAY_RAY_H_