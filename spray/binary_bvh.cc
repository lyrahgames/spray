#include "binary_bvh.h"
#include <stdexcept>

namespace spray {

Binary_bvh::Binary_bvh(Scene& scene) : scene_(&scene) {
  if (scene.primitive_data().size() == 0)
    std::runtime_error(
        "Cannot build Binary_bvh! Scene does not contain any primitives.");

  const Bounding_box box = bounds(scene);
  // compute morton codes for primitives
  std::vector<morton::primitive<int>> morton_data(
      scene.primitive_data().size());
  for (size_t i = 0; i < scene.primitive_data().size(); ++i) {
    const Eigen::Vector3f primitive_center =
        0.33333333f *
        (scene.vertex_data()[scene.primitive_data()[i].vertex_id[0]].position +
         scene.vertex_data()[scene.primitive_data()[i].vertex_id[1]].position +
         scene.vertex_data()[scene.primitive_data()[i].vertex_id[2]].position);

    const Eigen::Array3f tmp =
        static_cast<float>(1 << 10) *
        (static_cast<Eigen::Array3f>(primitive_center - box.min()) /
         static_cast<Eigen::Array3f>(box.max() - box.min()));

    uint32_t x_index = static_cast<uint32_t>(std::floor(tmp(0)));
    uint32_t y_index = static_cast<uint32_t>(std::floor(tmp(1)));
    uint32_t z_index = static_cast<uint32_t>(std::floor(tmp(2)));

    if (x_index == (1 << 10)) --x_index;
    if (y_index == (1 << 10)) --y_index;
    if (z_index == (1 << 10)) --z_index;

    morton_data[i].data = i;
    morton_data[i].code = morton::encode(x_index, y_index, z_index);
  }

  morton::radix_sort(morton_data);

  // sort primitives
  std::vector<Scene::primitive> sorted_primitive_data(
      scene.primitive_data().size());
  for (size_t i = 0; i < scene.primitive_data().size(); ++i) {
    sorted_primitive_data[i] = scene.primitive_data()[morton_data[i].data];
  }
  scene.primitive_data().swap(sorted_primitive_data);

  // build tree
  build_morton_bvh_node(3 * 10, 0, scene.primitive_data().size(), morton_data);
}

void Binary_bvh::build_morton_bvh() {}

int Binary_bvh::build_morton_bvh_node(
    int high_bit, int offset, int count,
    const std::vector<morton::primitive<int>>& morton_data) {
  if (high_bit == 0) {
    Binary_bvh::node n;
    n.child[0] = n.child[1] = -1;
    n.offset = offset;
    n.count = count;

    n.box = Bounding_box(
        scene()
            .vertex_data()[scene().primitive_data()[offset].vertex_id[0]]
            .position);
    n.box = Bounding_box(
        n.box, scene()
                   .vertex_data()[scene().primitive_data()[offset].vertex_id[1]]
                   .position);
    n.box = Bounding_box(
        n.box, scene()
                   .vertex_data()[scene().primitive_data()[offset].vertex_id[2]]
                   .position);

    for (int i = offset + 1; i < offset + count; ++i) {
      n.box = Bounding_box(
          n.box, scene()
                     .vertex_data()[scene().primitive_data()[i].vertex_id[0]]
                     .position);
      n.box = Bounding_box(
          n.box, scene()
                     .vertex_data()[scene().primitive_data()[i].vertex_id[1]]
                     .position);
      n.box = Bounding_box(
          n.box, scene()
                     .vertex_data()[scene().primitive_data()[i].vertex_id[2]]
                     .position);
    }

    node_data_.push_back(n);
    return node_data_.size() - 1;
  }

  const uint32_t mask = 1 << (high_bit - 1);
  int split = offset + count;
  for (int i = offset; i < offset + count; ++i) {
    if (morton_data[i].code & mask) {
      split = i;
      break;
    }
  }

  const int diff = split - offset;

  if (diff == 0 || diff == count) {
    const int index =
        build_morton_bvh_node(high_bit - 1, offset, count, morton_data);
    return index;
  }

  node_data_.push_back(Binary_bvh::node{});
  const int index = node_data_.size() - 1;

  node_data_[index].offset = offset;
  node_data_[index].count = count;

  // one has to define these extra const int, otherwise compiler makes wrong
  // optimizations
  const int child0_index =
      build_morton_bvh_node(high_bit - 1, offset, diff, morton_data);
  const int child1_index =
      build_morton_bvh_node(high_bit - 1, split, count - diff, morton_data);

  node_data_[index].child[0] = child0_index;
  node_data_[index].child[1] = child1_index;
  node_data_[index].box =
      Bounding_box(node_data_[child0_index].box, node_data_[child1_index].box);

  return index;
}

}  // namespace spray