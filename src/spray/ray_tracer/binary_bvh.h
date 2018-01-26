#ifndef SPRAY_RAY_TRACER_BINARY_BVH_H_
#define SPRAY_RAY_TRACER_BINARY_BVH_H_

#include <Eigen/Dense>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "bounding_box.h"
#include "morton.h"
#include "scene.h"

namespace spray {
namespace ray_tracer {

class Binary_bvh {
 public:
  Binary_bvh() = default;
  Binary_bvh(Scene& scene);

  struct node {
    Bounding_box box;
    int child[2];
    int offset;
    int count;

    bool is_leaf() const { return (child[0] == -1); }
  };

  static bool is_leaf(const node& n) { return (n.child[0] == -1); }

  const Scene& scene() const { return *scene_; }
  const std::vector<node>& node_data() const { return node_data_; }

 private:
  void build_morton_bvh();
  int build_morton_bvh_node(
      int high_bit, int offset, int count,
      const std::vector<morton::primitive<int>>& morton_data);

  const Scene* scene_;
  std::vector<node> node_data_;
};

}  // namespace ray_tracer
}  // namespace spray

#endif  // SPRAY_RAY_TRACER_BINARY_BVH_H_