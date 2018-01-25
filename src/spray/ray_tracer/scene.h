#ifndef SPRAY_RAY_TRACER_SCENE_H_
#define SPRAY_RAY_TRACER_SCENE_H_

#include <Eigen/Dense>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "bounding_box.h"
#include "morton.h"

namespace spray {
namespace ray_tracer {

class Scene {
 public:
  struct vertex {
    Eigen::Vector3f position;
    Eigen::Vector3f normal;
  };

  struct primitive {
    int vertex_id[3];
    Eigen::Vector3f normal;
  };

  struct binary_bvh {
    struct node {
      Bounding_box box;
      int child[2];
      int offset;
      int count;

      bool is_leaf() const { return (child[0] == -1); }
    };

    static bool is_leaf(const node& n) { return (n.child[0] == -1); }

    std::vector<node> node_data;
  };

  // aabb bounds() const;
  void build_morton_bvh();
  int build_morton_bvh_node(
      int high_bit, int offset, int count,
      const std::vector<morton::primitive<int>>& morton_data);

  std::vector<vertex> vertex_data;
  std::vector<primitive> primitive_data;
  binary_bvh bvh;
};

Bounding_box bounds(const Scene& s);
Scene load_stl(const std::string& file_name);
// Scene::binary_bvh morton_bvh(const Scene& s);

}  // namespace ray_tracer
}  // namespace spray

#endif  // SPRAY_RAY_TRACER_SCENE_H_