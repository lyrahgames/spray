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
  Eigen::Vector3f clear_color;

  void render() {
    accum_buffer.resize(cam.screen_width() * cam.screen_height());

#pragma omp parallel for
    for (int i = 0; i < cam.screen_height(); ++i) {
      for (int j = 0; j < cam.screen_width(); ++j) {
        const int index = cam.screen_width() * i + j;
        const ray r = primary_ray(cam, j, i);
        accum_buffer[index] = Eigen::Vector4f(clear_color(0), clear_color(1),
                                              clear_color(2), 1.0f);

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