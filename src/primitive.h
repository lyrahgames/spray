#ifndef SPRAY_PRIMITIVE_H_
#define SPRAY_PRIMITIVE_H_

#include <Eigen/Core>

namespace spray {
namespace ray_tracer {

struct primitive {
  Eigen::Vector3f vertex[3];
  Eigen::Vector3f normal;
};

}  // namespace ray_tracer
}  // namespace spray

#endif  // SPRAY_PRIMITIVE_H_