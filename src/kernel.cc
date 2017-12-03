#include "kernel.h"

namespace spray {
namespace ray_tracer {

std::vector<Eigen::Vector3f> render(const scene& s, const camera& cam) {
  std::vector<Eigen::Vector3f> buffer(cam.screen_width() * cam.screen_height());
  const aabb box = bounds(s);

#pragma omp parallel for
  for (int i = 0; i < cam.screen_height(); ++i) {
    for (int j = 0; j < cam.screen_width(); ++j) {
      const int index = cam.screen_width() * i + j;
      const ray r = primary_ray(cam, j, i);
      buffer[index] = Eigen::Vector3f(0, 0, 0);

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
          buffer[index] = Eigen::Vector3f(dot, dot, dot);
        }
      }
    }
  }

  return buffer;
}

std::vector<Eigen::Vector3f> render(const binary_bvh& bvh, const camera& cam) {
  std::vector<Eigen::Vector3f> buffer(cam.screen_width() * cam.screen_height());
  const aabb box = bounds(*bvh.mesh);

#pragma omp parallel for
  for (int i = 0; i < cam.screen_height(); ++i) {
    for (int j = 0; j < cam.screen_width(); ++j) {
      const int index = cam.screen_width() * i + j;
      const ray r = primary_ray(cam, j, i);
      buffer[index] = Eigen::Vector3f(0, 0, 0);

      int pid = -1;
      Eigen::Vector3f uvt(0.0f, 0.0f, INFINITY);

      traverse(bvh, r, pid, uvt);

      if (pid != -1) {
        const float dot =
            std::abs(-r.direction.dot(bvh.mesh->primitive_data[pid].normal));
        // if (dot < 0.0f) dot = 0.0f;
        buffer[index] = Eigen::Vector3f(dot, dot, dot);
      }
    }
  }

  return buffer;
}

void traverse(const binary_bvh& bvh, const ray& r, int& pid,
              Eigen::Vector3f& uvt) {
  traverse_node(bvh, r, 0, pid, uvt);
}

void traverse_node(const binary_bvh& bvh, const ray& r, int node_index,
                   int& pid, Eigen::Vector3f& uvt) {
  if (bvh.node_data[node_index].is_leaf()) {
    if (intersect(r, bvh.node_data[node_index].box)) {
      const int offset = bvh.node_data[node_index].offset;
      const int count = bvh.node_data[node_index].count;

      for (int p = offset; p < offset + count; ++p) {
        Eigen::Vector3f tmp_uvt(0.0f, 0.0f, INFINITY);
        const scene::primitive& prim =
            bvh.mesh->primitive_data[bvh.morton_data[p].data];

        if (intersect(r, bvh.mesh->vertex_data[prim.vertex_id[0]].position,
                      bvh.mesh->vertex_data[prim.vertex_id[1]].position,
                      bvh.mesh->vertex_data[prim.vertex_id[2]].position,
                      tmp_uvt)) {
          if (tmp_uvt(2) < uvt(2)) {
            uvt = tmp_uvt;
            pid = bvh.morton_data[p].data;
          }
        }
      }
    }

    return;
  }

  if (intersect(r, bvh.node_data[node_index].box)) {
    traverse_node(bvh, r, bvh.node_data[node_index].child[0], pid, uvt);
    traverse_node(bvh, r, bvh.node_data[node_index].child[1], pid, uvt);
  }
}

}  // namespace ray_tracer
}  // namespace spray