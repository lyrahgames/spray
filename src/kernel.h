#ifndef SPRAY_KERNEL_H_
#define SPRAY_KERNEL_H_

#include "aabb.h"
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

  void render();
  void render_bvh();
  void traverse(const ray& r, int* pid, Eigen::Vector3f* uvt);
  void traverse_node(const ray& r, int node_index, int* pid,
                     Eigen::Vector3f* uvt);
};

}  // namespace ray_tracer
}  // namespace spray

#endif  // SPRAY_KERNEL_H_