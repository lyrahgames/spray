#include "scene.h"

namespace spray {
namespace ray_tracer {

// aabb scene::bounds() const { return ray_tracer::bounds(*this); }

void scene::build_morton_bvh() {
  if (primitive_data.size() == 0) return;

  const aabb bounding_box = bounds(*this);

  // compute morton codes for primitives
  std::vector<morton::primitive<int>> morton_data(primitive_data.size());

  for (size_t i = 0; i < primitive_data.size(); ++i) {
    const Eigen::Vector3f primitive_center =
        0.33333333f * (vertex_data[primitive_data[i].vertex_id[0]].position +
                       vertex_data[primitive_data[i].vertex_id[1]].position +
                       vertex_data[primitive_data[i].vertex_id[2]].position);

    const Eigen::Array3f tmp =
        static_cast<float>(1 << 10) *
        (static_cast<Eigen::Array3f>(primitive_center - bounding_box.min) /
         static_cast<Eigen::Array3f>(bounding_box.max - bounding_box.min));

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
  std::vector<primitive> sorted_primitive_data(primitive_data.size());
  for (size_t i = 0; i < primitive_data.size(); ++i) {
    sorted_primitive_data[i] = primitive_data[morton_data[i].data];
  }
  primitive_data.swap(sorted_primitive_data);

  // build tree
  build_morton_bvh_node(3 * 10, 0, primitive_data.size(), morton_data);
}

int scene::build_morton_bvh_node(
    int high_bit, int offset, int count,
    const std::vector<morton::primitive<int>>& morton_data) {
  if (high_bit == 0) {
    binary_bvh::node n;
    n.child[0] = n.child[1] = -1;
    n.offset = offset;
    n.count = count;

    n.box = aabb{Eigen::Vector3f(INFINITY, INFINITY, INFINITY),
                 Eigen::Vector3f(-INFINITY, -INFINITY, -INFINITY)};

    for (int i = offset; i < offset + count; ++i) {
      n.box =
          bounds(n.box, vertex_data[primitive_data[i].vertex_id[0]].position);
      n.box =
          bounds(n.box, vertex_data[primitive_data[i].vertex_id[1]].position);
      n.box =
          bounds(n.box, vertex_data[primitive_data[i].vertex_id[2]].position);
    }

    bvh.node_data.push_back(n);
    return bvh.node_data.size() - 1;
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

  bvh.node_data.push_back(binary_bvh::node{});
  const int index = bvh.node_data.size() - 1;

  bvh.node_data[index].offset = offset;
  bvh.node_data[index].count = count;

  // one has to define these extra const int, otherwise compiler makes wrong
  // optimizations
  const int child0_index =
      build_morton_bvh_node(high_bit - 1, offset, diff, morton_data);
  const int child1_index =
      build_morton_bvh_node(high_bit - 1, split, count - diff, morton_data);

  bvh.node_data[index].child[0] = child0_index;
  bvh.node_data[index].child[1] = child1_index;
  bvh.node_data[index].box =
      bounds(bvh.node_data[child0_index].box, bvh.node_data[child1_index].box);

  return index;
}

aabb bounds(const scene& s) {
  aabb box{Eigen::Vector3f(INFINITY, INFINITY, INFINITY),
           Eigen::Vector3f(-INFINITY, -INFINITY, -INFINITY)};

  for (size_t i = 0; i < s.vertex_data.size(); ++i) {
    box = bounds(box, s.vertex_data[i].position);
  }

  return box;
}

scene load_stl(const std::string& file_name) {
  std::fstream in(file_name, std::ios::binary | std::ios::in);

  if (!in.is_open()) {
    std::cerr << "File \"" << file_name << "\" could not be opened!"
              << std::endl;
    in.close();
    return scene{};
  }

  // ignore file header
  in.ignore(80);

  std::uint32_t primitive_count;
  in.read(reinterpret_cast<char*>(&primitive_count), 4);

  scene s;
  s.primitive_data.resize(primitive_count);
  s.vertex_data.resize(3 * primitive_count);

  for (std::uint32_t i = 0; i < primitive_count; i++) {
    in.read(reinterpret_cast<char*>(&s.primitive_data[i].normal), 12);

    in.read(reinterpret_cast<char*>(&s.vertex_data[3 * i + 0].position), 12);
    in.read(reinterpret_cast<char*>(&s.vertex_data[3 * i + 1].position), 12);
    in.read(reinterpret_cast<char*>(&s.vertex_data[3 * i + 2].position), 12);

    s.primitive_data[i].vertex_id[0] = 3 * i;
    s.primitive_data[i].vertex_id[1] = 3 * i + 1;
    s.primitive_data[i].vertex_id[2] = 3 * i + 2;

    // ignore attribute byte count
    in.ignore(2);

    // test if the normal is near zero and compute if this is the case
    if (s.primitive_data[i].normal.norm() <= 0.1f) {
      s.primitive_data[i].normal =
          (s.vertex_data[3 * i + 1].position -
           s.vertex_data[3 * i + 0].position)
              .cross(s.vertex_data[3 * i + 2].position -
                     s.vertex_data[3 * i + 0].position);
      s.primitive_data[i].normal.normalize();
    }
  }

  return s;
}

}  // namespace ray_tracer
}  // namespace spray