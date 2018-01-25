#include "kernel.h"

namespace spray {
namespace ray_tracer {

Kernel::Kernel()
    : rng_(std::mt19937{std::random_device{}()}), sample_count_{0} {}

void Kernel::reset_cache() {
  sample_count_ = 0;
  pixel_buffer_.resize(camera().screen_width() * camera().screen_height());
  pixel_buffer_.assign(pixel_buffer_.size(),
                       Eigen::Vector4f(0.0f, 0.0f, 0.0f, 0.0f));
}

void Kernel::render() {
  pixel_buffer_.resize(camera().screen_width() * camera().screen_height());

  // const Bounding_box box(scene().min, scene().max);
  const Bounding_box box = bounds(scene());

#pragma omp parallel for
  for (int i = 0; i < camera().screen_height(); ++i) {
    for (int j = 0; j < camera().screen_width(); ++j) {
      const int index = camera().screen_width() * i + j;
      const Ray r = primary_ray(camera(), j, i);
      pixel_buffer_[index] = Eigen::Vector4f(clear_color_(0), clear_color_(1),
                                             clear_color_(2), 1.0f);

      if (intersect(r, box)) {
        int pid = -1;
        Eigen::Vector3f uvt(0.0f, 0.0f, INFINITY);

        for (int p = 0; p < static_cast<int>(scene().primitive_data.size());
             ++p) {
          Eigen::Vector3f tmp_uvt;

          const Scene::primitive& prim = scene().primitive_data[p];

          if (intersect(r, scene().vertex_data[prim.vertex_id[0]].position,
                        scene().vertex_data[prim.vertex_id[1]].position,
                        scene().vertex_data[prim.vertex_id[2]].position,
                        tmp_uvt)) {
            if (tmp_uvt(2) < uvt(2)) {
              uvt = tmp_uvt;
              pid = p;
            }
          }
        }

        if (pid != -1) {
          float dot = -r.direction().dot(scene().primitive_data[pid].normal);
          if (dot < 0.0f) dot = 0.0f;
          pixel_buffer_[index] = Eigen::Vector4f(dot, dot, dot, 1.0f);
        }
      }
    }
  }
}

void Kernel::render_bvh() {
  // pixel_buffer_.resize(camera().screen_width() * camera().screen_height());

  ++sample_count_;

#pragma omp parallel for
  for (int i = 0; i < camera().screen_height(); ++i) {
    for (int j = 0; j < camera().screen_width(); ++j) {
      const int index = camera().screen_width() * i + j;
      // const Ray r = primary_ray(camera(), j, i);
      const Ray r = jittered_primary_ray(camera(), j, i, rng_);
      const Cached_ray traversal_ray(r);
      Eigen::Vector4f color(clear_color_(0), clear_color_(1), clear_color_(2),
                            1.0f);

      int pid = -1;
      Eigen::Vector3f uvt(0.0f, 0.0f, INFINITY);

      // traverse(r, &pid, &uvt);
      traverse(traversal_ray, &pid, &uvt);

      if (pid != -1) {
        const float dot =
            std::abs(-r.direction().dot(scene().primitive_data[pid].normal));
        color = Eigen::Vector4f(dot, dot, dot, 1);
      }

      pixel_buffer_[index] =
          (static_cast<float>(sample_count_ - 1) * pixel_buffer_[index] +
           color) /
          static_cast<float>(sample_count_);
    }
  }
}

void Kernel::traverse(const Ray& r, int* pid, Eigen::Vector3f* uvt) {
  traverse_node(r, 0, pid, uvt);
}

void Kernel::traverse_node(const Ray& r, int node_index, int* pid,
                           Eigen::Vector3f* uvt) {
  if (Scene::binary_bvh::is_leaf(scene().bvh.node_data[node_index])) {
    if (intersect(r, scene().bvh.node_data[node_index].box)) {
      const int offset = scene().bvh.node_data[node_index].offset;
      const int count = scene().bvh.node_data[node_index].count;

      for (int p = offset; p < offset + count; ++p) {
        Eigen::Vector3f tmp_uvt(0.0f, 0.0f, INFINITY);
        const Scene::primitive& prim = scene().primitive_data[p];

        if (intersect(r, scene().vertex_data[prim.vertex_id[0]].position,
                      scene().vertex_data[prim.vertex_id[1]].position,
                      scene().vertex_data[prim.vertex_id[2]].position,
                      tmp_uvt)) {
          if (tmp_uvt(2) < (*uvt)(2)) {
            *uvt = tmp_uvt;
            *pid = p;
          }
        }
      }
    }

    return;
  }

  if (intersect(r, scene().bvh.node_data[node_index].box)) {
    traverse_node(r, scene().bvh.node_data[node_index].child[0], pid, uvt);
    traverse_node(r, scene().bvh.node_data[node_index].child[1], pid, uvt);
  }
}

void Kernel::traverse(const Cached_ray& r, int* pid, Eigen::Vector3f* uvt) {
  traverse_node(r, 0, pid, uvt);
}

void Kernel::traverse_node(const Cached_ray& r, int node_index, int* pid,
                           Eigen::Vector3f* uvt) {
  if (Scene::binary_bvh::is_leaf(scene().bvh.node_data[node_index])) {
    float tmp_t;
    if (intersect(r, scene().bvh.node_data[node_index].box, &tmp_t) &&
        tmp_t < (*uvt)(2)) {
      const int offset = scene().bvh.node_data[node_index].offset;
      const int count = scene().bvh.node_data[node_index].count;

      for (int p = offset; p < offset + count; ++p) {
        Eigen::Vector3f tmp_uvt(0.0f, 0.0f, INFINITY);
        const Scene::primitive& prim = scene().primitive_data[p];

        if (intersect(r, scene().vertex_data[prim.vertex_id[0]].position,
                      scene().vertex_data[prim.vertex_id[1]].position,
                      scene().vertex_data[prim.vertex_id[2]].position,
                      tmp_uvt)) {
          if (tmp_uvt(2) < (*uvt)(2)) {
            *uvt = tmp_uvt;
            *pid = p;
          }
        }
      }
    }

    return;
  }

  if (intersect(r, scene().bvh.node_data[node_index].box)) {
    traverse_node(r, scene().bvh.node_data[node_index].child[0], pid, uvt);
    traverse_node(r, scene().bvh.node_data[node_index].child[1], pid, uvt);
  }
}

}  // namespace ray_tracer
}  // namespace spray