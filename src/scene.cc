#include "scene.h"

namespace spray {
namespace ray_tracer {

scene load_stl(std::string file_name) {
  std::fstream in(file_name);

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

    in.read(reinterpret_cast<char*>(&s.vertex_data[3 * i].position), 12);
    in.read(reinterpret_cast<char*>(&s.vertex_data[3 * i + 1].position), 12);
    in.read(reinterpret_cast<char*>(&s.vertex_data[3 * i + 2].position), 12);

    s.primitive_data[i].vertex_id[0] = 3 * i;
    s.primitive_data[i].vertex_id[1] = 3 * i + 1;
    s.primitive_data[i].vertex_id[2] = 3 * i + 2;

    // ignore attribute byte count
    in.ignore(2);
  }

  // compute bounds and parameters
  Eigen::Array3f tmp_max(-INFINITY, -INFINITY, -INFINITY);
  Eigen::Array3f tmp_min(INFINITY, INFINITY, INFINITY);

  for (int i = 0; i < static_cast<int>(s.vertex_data.size()); ++i) {
    tmp_max =
        tmp_max.max(static_cast<Eigen::Array3f>(s.vertex_data[i].position));

    tmp_min =
        tmp_min.min(static_cast<Eigen::Array3f>(s.vertex_data[i].position));
  }

  s.max = static_cast<Eigen::Vector3f>(tmp_max);
  s.min = static_cast<Eigen::Vector3f>(tmp_min);

  s.center = 0.5f * (s.max + s.min);
  s.radius = 0.5f * (s.max - s.min).norm();

  return s;
}

}  // namespace ray_tracer
}  // namespace spray