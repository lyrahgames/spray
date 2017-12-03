#ifndef SPRAY_BINARY_BVH_H_
#define SPRAY_BINARY_BVH_H_

#include <vector>
#include "aabb.h"
#include "morton.h"
#include "scene.h"

namespace spray {
namespace ray_tracer {

struct binary_bvh {
  struct node {
    aabb box;
    int child[2];
    int offset;
    int count;
    int split;

    bool is_leaf() const { return child[0] == -1; }
  };

  int build_node(int high_bit, int offset, int count);

  std::vector<node> node_data;
  std::vector<morton::primitive<int>> morton_data;
  scene* mesh;
};

binary_bvh build(scene& s);
void build(scene& s, binary_bvh& bvh);

inline void print(const binary_bvh& bvh) {
  for (int i = 0; i < bvh.node_data.size(); ++i) {
    std::cout << i << ": " << bvh.node_data[i].child[0] << ", "
              << bvh.node_data[i].child[1] << std::endl;
  }
}

}  // namespace ray_tracer
}  // namespace spray

#endif  // SPRAY_BINARY_BVH_H_