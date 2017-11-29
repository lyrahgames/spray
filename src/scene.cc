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

  scene result;
  result.primitive_vector.resize(primitive_count);

  for (std::uint32_t i = 0; i < primitive_count; i++) {
    in.read(reinterpret_cast<char*>(&result.primitive_vector[i].normal), 12);
    in.read(reinterpret_cast<char*>(&result.primitive_vector[i].vertex[0]), 12);
    in.read(reinterpret_cast<char*>(&result.primitive_vector[i].vertex[1]), 12);
    in.read(reinterpret_cast<char*>(&result.primitive_vector[i].vertex[2]), 12);
    // ignore attribute byte count
    in.ignore(2);
  }

  // compute bounds and parameters
  Eigen::Array3f tmp_max(-INFINITY, -INFINITY, -INFINITY);
  Eigen::Array3f tmp_min(INFINITY, INFINITY, INFINITY);

  for (int i = 0; i < static_cast<int>(primitive_count); ++i) {
    tmp_max = tmp_max.max(
        static_cast<Eigen::Array3f>(result.primitive_vector[i].vertex[0]));
    tmp_max = tmp_max.max(
        static_cast<Eigen::Array3f>(result.primitive_vector[i].vertex[1]));
    tmp_max = tmp_max.max(
        static_cast<Eigen::Array3f>(result.primitive_vector[i].vertex[2]));

    tmp_min = tmp_min.min(
        static_cast<Eigen::Array3f>(result.primitive_vector[i].vertex[0]));
    tmp_min = tmp_min.min(
        static_cast<Eigen::Array3f>(result.primitive_vector[i].vertex[1]));
    tmp_min = tmp_min.min(
        static_cast<Eigen::Array3f>(result.primitive_vector[i].vertex[2]));
  }

  result.max = static_cast<Eigen::Vector3f>(tmp_max);
  result.min = static_cast<Eigen::Vector3f>(tmp_min);

  result.center = 0.5f * (result.max + result.min);
  result.radius = 0.5f * (result.max - result.min).norm();

  return result;
}

}  // namespace ray_tracer
}  // namespace spray