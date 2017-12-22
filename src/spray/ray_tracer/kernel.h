#ifndef SPRAY_RAY_TRACER_KERNEL_H_
#define SPRAY_RAY_TRACER_KERNEL_H_

#include <random>
#include "bounding_box.h"
#include "camera.h"
#include "intersection.h"
#include "scene.h"

namespace spray {
namespace ray_tracer {

struct Kernel {
  std::vector<Eigen::Vector4f> accum_buffer;
  Camera cam;
  Scene s;
  Eigen::Vector3f clear_color;
  std::mt19937 rng;
  int sample_count;

  static constexpr int max_sample_count = 16;

  void reset();

  void render();
  void render_bvh();
  void traverse(const Ray& r, int* pid, Eigen::Vector3f* uvt);
  void traverse_node(const Ray& r, int node_index, int* pid,
                     Eigen::Vector3f* uvt);
  void traverse(const Cached_ray& r, int* pid, Eigen::Vector3f* uvt);
  void traverse_node(const Cached_ray& r, int node_index, int* pid,
                     Eigen::Vector3f* uvt);
};

}  // namespace ray_tracer
}  // namespace spray

#endif  // SPRAY_RAY_TRACER_KERNEL_H_