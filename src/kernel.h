#ifndef SPRAY_KERNEL_H_
#define SPRAY_KERNEL_H_

#include "camera.h"
#include "intersection.h"
#include "scene.h"

namespace spray {
namespace ray_tracer {

struct kernel {
  std::vector<Eigen::Vector3f> accum_buffer;
  camera cam;
  scene s;

  void render() {
    accum_buffer.resize(cam.pixel_cols() * cam.pixel_rows());

    for (int i = 0; i < cam.pixel_rows(); ++i) {
      for (int j = 0; j < cam.pixel_cols(); ++i) {
        const int index = cam.pixel_cols() * i + j;
        const ray r = cam.primary_ray(j, i);
        accum_buffer[index] = Eigen::Vector3f(0.0f, 0.0f, 0.0f);

        for (int p = 0; p < static_cast<int>(s.primitive_vector.size()); ++p) {
          if (intersect(r, s.primitive_vector[p])) {
            accum_buffer[index] = Eigen::Vector3f(1.0f, 1.0f, 1.0f);
            break;
          }
        }
      }
    }
  }
};

}  // namespace ray_tracer
}  // namespace spray

#endif  // SPRAY_KERNEL_H_