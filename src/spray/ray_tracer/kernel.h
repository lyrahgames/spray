#ifndef SPRAY_RAY_TRACER_KERNEL_H_
#define SPRAY_RAY_TRACER_KERNEL_H_

#include <random>
#include "bounding_box.h"
#include "camera.h"
#include "intersection.h"
#include "scene.h"

namespace spray {
namespace ray_tracer {

class Kernel {
 public:
  Kernel();

  const std::vector<Eigen::Vector4f>& pixel_buffer() const {
    return pixel_buffer_;
  }
  const Camera& camera() const { return *camera_; }
  const Scene& scene() const { return *scene_; }
  const Eigen::Vector3f& clear_color() const { return clear_color_; }

  void set_camera(const Camera* c) { camera_ = c; }
  void set_scene(const Scene* s) { scene_ = s; }
  void reset_cache();

  void render();
  void render_bvh();
  void traverse(const Ray& r, int* pid, Eigen::Vector3f* uvt);
  void traverse_node(const Ray& r, int node_index, int* pid,
                     Eigen::Vector3f* uvt);
  void traverse(const Cached_ray& r, int* pid, Eigen::Vector3f* uvt);
  void traverse_node(const Cached_ray& r, int node_index, int* pid,
                     Eigen::Vector3f* uvt);

 private:
  std::vector<Eigen::Vector4f> pixel_buffer_;
  const Camera* camera_;
  const Scene* scene_;
  Eigen::Vector3f clear_color_;
  std::mt19937 rng_;
  int sample_count_;
};

}  // namespace ray_tracer
}  // namespace spray

#endif  // SPRAY_RAY_TRACER_KERNEL_H_