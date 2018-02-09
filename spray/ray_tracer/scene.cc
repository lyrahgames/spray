#include "scene.h"
#include <cmath>
#include <cstdint>
#include <fstream>
#include <stdexcept>

namespace spray {
namespace ray_tracer {

Scene::Scene(const std::string& file_path) {
  std::fstream file(file_path, std::ios::binary | std::ios::in);

  if (!file.is_open()) {
    std::string what("The file '");
    what += file_path;
    what += "' could not be opened! Does it exist?";
    throw std::runtime_error(what);
  }

  file.ignore(80);
  std::uint32_t primitive_count;
  file.read(reinterpret_cast<char*>(&primitive_count), 4);

  primitive_data_.resize(primitive_count);
  vertex_data_.resize(3 * primitive_count);

  for (std::uint32_t i = 0; i < primitive_count; ++i) {
    file.read(reinterpret_cast<char*>(&primitive_data_[i].normal), 12);
    file.read(reinterpret_cast<char*>(&vertex_data_[3 * i + 0].position), 12);
    file.read(reinterpret_cast<char*>(&vertex_data_[3 * i + 1].position), 12);
    file.read(reinterpret_cast<char*>(&vertex_data_[3 * i + 2].position), 12);
    primitive_data_[i].vertex_id[0] = 3 * i;
    primitive_data_[i].vertex_id[1] = 3 * i + 1;
    primitive_data_[i].vertex_id[2] = 3 * i + 2;
    file.ignore(2);

    // test if the normal is near zero and compute if this is the case
    if (primitive_data_[i].normal.norm() <= 0.1f) {
      primitive_data_[i].normal =
          (vertex_data_[3 * i + 1].position - vertex_data_[3 * i + 0].position)
              .cross(vertex_data_[3 * i + 2].position -
                     vertex_data_[3 * i + 0].position);
      primitive_data_[i].normal.normalize();
    }

    primitive_data_[i].normal_id[0] = normal_data_.size();
    primitive_data_[i].normal_id[1] = normal_data_.size();
    primitive_data_[i].normal_id[2] = normal_data_.size();
    normal_data_.push_back(primitive_data_[i].normal);
  }
}

Bounding_box bounds(const Scene& scene) {
  Bounding_box box(scene.vertex_data()[0].position);

  for (size_t i = 1; i < scene.vertex_data().size(); ++i) {
    box = Bounding_box(box, scene.vertex_data()[i].position);
  }

  return box;
}

}  // namespace ray_tracer
}  // namespace spray