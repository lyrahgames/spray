#ifndef SPRAY_PRIMITIVE_H_
#define SPRAY_PRIMITIVE_H_

#include <Eigen/Core>

namespace spray {
namespace ray_trace {

struct primitive {
  Eigen::Vector3f vertex[3];
  Eigen::Vector3f normal;
};

}  // namespace ray_trace
}  // namespace spray

#endif  // SPRAY_PRIMITIVE_H_