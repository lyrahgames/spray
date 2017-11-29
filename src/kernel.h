#ifndef SPRAY_KERNEL_H_
#define SPRAY_KERNEL_H_

#include "camera.h"
#include "intersection.h"
#include "scene.h"

namespace spray {
namespace ray_tracer {

struct kernel {
  std::vector<Eigen::Vector4f> accum_buffer;
  camera cam;
  scene s;

  void clear() {
    accum_buffer.resize(cam.pixel_cols() * cam.pixel_rows());

    for (int i = 0; i < static_cast<int>(accum_buffer.size()); ++i) {
      accum_buffer[i] = Eigen::Vector4f(0.0f, 0.5f, 1.0f, 1);
    }
  }

  void render() {
    accum_buffer.resize(cam.pixel_cols() * cam.pixel_rows());

#pragma omp parallel for
    for (int i = 0; i < cam.pixel_rows(); ++i) {
      for (int j = 0; j < cam.pixel_cols(); ++j) {
        const int index = cam.pixel_cols() * i + j;
        const ray r = cam.primary_ray(j, i);
        // accum_buffer[index] = Eigen::Vector4f(0.1f, 0.1f, 0.1f, 1.0f);
        accum_buffer[index] = Eigen::Vector4f(0.2f, 0.2f, 0.2f, 1.0f);

        int pid = -1;
        Eigen::Vector3f uvt(0.0f, 0.0f, INFINITY);

        for (int p = 0; p < static_cast<int>(s.primitive_vector.size()); ++p) {
          Eigen::Vector3f tmp_uvt;

          if (intersect(r, s.primitive_vector[p].vertex[0],
                        s.primitive_vector[p].vertex[1],
                        s.primitive_vector[p].vertex[2], tmp_uvt)) {
            if (tmp_uvt(2) < uvt(2)) {
              uvt = tmp_uvt;
              pid = p;
            }
          }
        }

        if (pid != -1) {
          float dot = -r.direction.dot(s.primitive_vector[pid].normal);
          if (dot < 0.0f) dot = 0.0f;
          accum_buffer[index] = Eigen::Vector4f(dot, dot, dot, 1.0f);
        }
      }
    }
  }
};

}  // namespace ray_tracer
}  // namespace spray

#endif  // SPRAY_KERNEL_H_