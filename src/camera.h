#ifndef SPRAY_CAMERA_H_
#define SPRAY_CAMERA_H_

#include <Eigen/Core>

namespace spray {
namespace ray_tracer {

struct camera {
  Eigen::Vector3f position;
  Eigen::Vector3f direction;
  Eigen::Vector3f up;
  float field_of_view;
  int pixel_rows;
  int pixel_cols;
};

}  // namespace ray_tracer
}  // namespace spray

#endif  // SPRAY_CAMERA_H_