#include "kernel.h"

namespace spray {
namespace ray_tracer {

void kernel::reset() {
  sample_count = 0;
  accum_buffer.resize(cam.screen_width() * cam.screen_height());
  accum_buffer.assign(accum_buffer.size(),
                      Eigen::Vector4f(0.0f, 0.0f, 0.0f, 0.0f));
}

void kernel::render() {
  accum_buffer.resize(cam.screen_width() * cam.screen_height());

  // const aabb box(s.min, s.max);
  const aabb box = bounds(s);

#pragma omp parallel for
  for (int i = 0; i < cam.screen_height(); ++i) {
    for (int j = 0; j < cam.screen_width(); ++j) {
      const int index = cam.screen_width() * i + j;
      const ray r = primary_ray(cam, j, i);
      accum_buffer[index] =
          Eigen::Vector4f(clear_color(0), clear_color(1), clear_color(2), 1.0f);

      if (intersect(r, box)) {
        int pid = -1;
        Eigen::Vector3f uvt(0.0f, 0.0f, INFINITY);

        for (int p = 0; p < static_cast<int>(s.primitive_data.size()); ++p) {
          Eigen::Vector3f tmp_uvt;

          const scene::primitive& prim = s.primitive_data[p];

          if (intersect(r, s.vertex_data[prim.vertex_id[0]].position,
                        s.vertex_data[prim.vertex_id[1]].position,
                        s.vertex_data[prim.vertex_id[2]].position, tmp_uvt)) {
            if (tmp_uvt(2) < uvt(2)) {
              uvt = tmp_uvt;
              pid = p;
            }
          }
        }

        if (pid != -1) {
          float dot = -r.direction.dot(s.primitive_data[pid].normal);
          if (dot < 0.0f) dot = 0.0f;
          accum_buffer[index] = Eigen::Vector4f(dot, dot, dot, 1.0f);
        }
      }
    }
  }
}

void kernel::render_bvh() {
  // accum_buffer.resize(cam.screen_width() * cam.screen_height());

  ++sample_count;

#pragma omp parallel for
  for (int i = 0; i < cam.screen_height(); ++i) {
    for (int j = 0; j < cam.screen_width(); ++j) {
      const int index = cam.screen_width() * i + j;
      // const ray r = primary_ray(cam, j, i);
      const ray r = jittered_primary_ray(cam, j, i, rng);
      const cached_ray traversal_ray(r);
      Eigen::Vector4f color(clear_color(0), clear_color(1), clear_color(2),
                            1.0f);

      int pid = -1;
      Eigen::Vector3f uvt(0.0f, 0.0f, INFINITY);

      // traverse(r, &pid, &uvt);
      traverse(traversal_ray, &pid, &uvt);

      if (pid != -1) {
        const float dot =
            std::abs(-r.direction.dot(s.primitive_data[pid].normal));
        color = Eigen::Vector4f(dot, dot, dot, 1);
      }

      accum_buffer[index] =
          (static_cast<float>(sample_count - 1) * accum_buffer[index] + color) /
          static_cast<float>(sample_count);
    }
  }
}

void kernel::traverse(const ray& r, int* pid, Eigen::Vector3f* uvt) {
  traverse_node(r, 0, pid, uvt);
}

void kernel::traverse_node(const ray& r, int node_index, int* pid,
                           Eigen::Vector3f* uvt) {
  if (scene::binary_bvh::is_leaf(s.bvh.node_data[node_index])) {
    if (intersect(r, s.bvh.node_data[node_index].box)) {
      const int offset = s.bvh.node_data[node_index].offset;
      const int count = s.bvh.node_data[node_index].count;

      for (int p = offset; p < offset + count; ++p) {
        Eigen::Vector3f tmp_uvt(0.0f, 0.0f, INFINITY);
        const scene::primitive& prim = s.primitive_data[p];

        if (intersect(r, s.vertex_data[prim.vertex_id[0]].position,
                      s.vertex_data[prim.vertex_id[1]].position,
                      s.vertex_data[prim.vertex_id[2]].position, tmp_uvt)) {
          if (tmp_uvt(2) < (*uvt)(2)) {
            *uvt = tmp_uvt;
            *pid = p;
          }
        }
      }
    }

    return;
  }

  if (intersect(r, s.bvh.node_data[node_index].box)) {
    traverse_node(r, s.bvh.node_data[node_index].child[0], pid, uvt);
    traverse_node(r, s.bvh.node_data[node_index].child[1], pid, uvt);
  }
}

void kernel::traverse(const cached_ray& r, int* pid, Eigen::Vector3f* uvt) {
  traverse_node(r, 0, pid, uvt);
}

void kernel::traverse_node(const cached_ray& r, int node_index, int* pid,
                           Eigen::Vector3f* uvt) {
  if (scene::binary_bvh::is_leaf(s.bvh.node_data[node_index])) {
    if (intersect(r, s.bvh.node_data[node_index].box)) {
      const int offset = s.bvh.node_data[node_index].offset;
      const int count = s.bvh.node_data[node_index].count;

      for (int p = offset; p < offset + count; ++p) {
        Eigen::Vector3f tmp_uvt(0.0f, 0.0f, INFINITY);
        const scene::primitive& prim = s.primitive_data[p];

        if (intersect(r, s.vertex_data[prim.vertex_id[0]].position,
                      s.vertex_data[prim.vertex_id[1]].position,
                      s.vertex_data[prim.vertex_id[2]].position, tmp_uvt)) {
          if (tmp_uvt(2) < (*uvt)(2)) {
            *uvt = tmp_uvt;
            *pid = p;
          }
        }
      }
    }

    return;
  }

  if (intersect(r, s.bvh.node_data[node_index].box)) {
    traverse_node(r, s.bvh.node_data[node_index].child[0], pid, uvt);
    traverse_node(r, s.bvh.node_data[node_index].child[1], pid, uvt);
  }
}

}  // namespace ray_tracer
}  // namespace spray