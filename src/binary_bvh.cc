#include "binary_bvh.h"

namespace spray {
namespace ray_tracer {

int binary_bvh::build_node(int high_bit, int offset, int count) {
  // std::cout << "build_node(" << high_bit << ", " << offset << ", " << count
  //           << ")" << std::endl;

  if (count == 0) return -1;

  if (high_bit == 0) {
    // std::cout << "build_node(" << high_bit << ", " << offset << ", " << count
    //           << ")" << std::endl;
    // std::cout << "bound primitves: offset = " << offset << ", count = " <<
    // count
    //           << std::endl;
    node n;
    n.child[0] = n.child[1] = -1;
    n.offset = offset;
    n.count = count;
    n.split = 0;
    n.box = aabb{Eigen::Vector3f(INFINITY, INFINITY, INFINITY),
                 Eigen::Vector3f(-INFINITY, -INFINITY, -INFINITY)};

    for (int i = offset; i < offset + count; ++i) {
      n.box = bounds(n.box,
                     mesh->vertex_data[mesh->primitive_data[morton_data[i].data]
                                           .vertex_id[0]]
                         .position);
      n.box = bounds(n.box,
                     mesh->vertex_data[mesh->primitive_data[morton_data[i].data]
                                           .vertex_id[1]]
                         .position);
      n.box = bounds(n.box,
                     mesh->vertex_data[mesh->primitive_data[morton_data[i].data]
                                           .vertex_id[2]]
                         .position);
    }

    node_data.push_back(n);
    // std::cout << "build_node(" << high_bit << ", " << offset << ", " << count
    //           << ") = " << node_data.size() - 1 << std::endl;
    return node_data.size() - 1;
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
    // std::cout << "build_node(" << high_bit << ", " << offset << ", " << count
    //           << ")" << std::endl;
    // std::cout << "forward" << std::endl;
    int tmp = build_node(high_bit - 1, offset, count);
    // std::cout << "build_node(" << high_bit << ", " << offset << ", " << count
    //           << ") = " << tmp << std::endl;
    return tmp;
  }

  node_data.push_back(node{});
  const int index = node_data.size() - 1;

  node& n = node_data[index];

  node_data[index].offset = offset;
  node_data[index].count = count;
  node_data[index].split = split;

  int tmp;

  // std::cout << index << ".build_node(" << high_bit << ", " << offset << ", "
  //           << count << ")" << std::endl;
  // std::cout << index << ".build children: split = " << split << std::endl;
  tmp = build_node(high_bit - 1, offset, diff);
  node_data[index].child[0] = tmp;
  // std::cout << high_bit << ".build_node(" << high_bit << ", " << offset << ",
  // "
  //           << count << ")" << std::endl;
  // std::cout << index << ".build children: split = " << split << std::endl;
  tmp = build_node(high_bit - 1, split, count - diff);
  node_data[index].child[1] = tmp;

  node_data[index].box = bounds(node_data[node_data[index].child[0]].box,
                                node_data[node_data[index].child[1]].box);

  // std::cout << "build_node(" << high_bit << ", " << offset << ", " << count
  //           << ") = " << index << std::endl;

  return index;
}

binary_bvh build(scene& s) {
  binary_bvh bvh;
  bvh.mesh = &s;

  const aabb bounding_box = bounds(s);

  std::cout << "generate morton codes..." << std::flush;

  bvh.morton_data.resize(s.primitive_data.size());
  for (size_t i = 0; i < s.primitive_data.size(); ++i) {
    const Eigen::Vector3f primitive_center =
        (s.vertex_data[s.primitive_data[i].vertex_id[0]].position +
         s.vertex_data[s.primitive_data[i].vertex_id[1]].position +
         s.vertex_data[s.primitive_data[i].vertex_id[2]].position) /
        3.0f;

    const Eigen::Array3f morton_index =
        1024.0f *
        (static_cast<Eigen::Array3f>(primitive_center - bounding_box.min) /
         static_cast<Eigen::Array3f>(bounding_box.max - bounding_box.min));

    bvh.morton_data[i].code =
        morton::encode(static_cast<uint32_t>(morton_index(0)),
                       static_cast<uint32_t>(morton_index(1)),
                       static_cast<uint32_t>(morton_index(2)));
    bvh.morton_data[i].data = i;
  }

  std::cout << "done." << std::endl;

  std::cout << "sort morton primitves..." << std::flush;
  morton::radix_sort(bvh.morton_data);
  std::cout << "done." << std::endl;

  std::cout << "build bvh..." << std::flush;
  bvh.build_node(30, 0, s.primitive_data.size());
  // bvh.build_node(30, 0, 1);
  std::cout << "done." << std::endl;

  std::cout << "node count: " << bvh.node_data.size() << std::endl;

  return bvh;
}

void build(scene& s, binary_bvh& bvh) {
  bvh.mesh = &s;

  const aabb bounding_box = bounds(s);

  std::cout << "generate morton codes..." << std::flush;

  bvh.morton_data.resize(s.primitive_data.size());
  for (size_t i = 0; i < s.primitive_data.size(); ++i) {
    const Eigen::Vector3f primitive_center =
        (0.333333f * s.vertex_data[s.primitive_data[i].vertex_id[0]].position +
         0.333333f * s.vertex_data[s.primitive_data[i].vertex_id[1]].position +
         0.333333f * s.vertex_data[s.primitive_data[i].vertex_id[2]].position);

    const Eigen::Array3f morton_index =
        1024.0f *
        (static_cast<Eigen::Array3f>(primitive_center - bounding_box.min) /
         static_cast<Eigen::Array3f>(bounding_box.max - bounding_box.min));

    bvh.morton_data[i].code =
        morton::encode(static_cast<uint32_t>(std::floor(morton_index(0))),
                       static_cast<uint32_t>(std::floor(morton_index(1))),
                       static_cast<uint32_t>(std::floor(morton_index(2))));
    bvh.morton_data[i].data = i;
  }

  std::cout << "done." << std::endl;

  std::cout << "sort morton primitves..." << std::flush;
  morton::radix_sort(bvh.morton_data);
  std::cout << "done." << std::endl;

  std::cout << "build bvh..." << std::flush;
  bvh.build_node(30, 0, s.primitive_data.size());
  // bvh.build_node(30, 0, 1);
  std::cout << "done." << std::endl;

  std::cout << "node count: " << bvh.node_data.size() << std::endl;
  // print(bvh);
}

}  // namespace ray_tracer
}  // namespace spray