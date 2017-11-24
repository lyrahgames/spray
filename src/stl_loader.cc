#include "stl_loader.h"

namespace spray {
namespace ray_trace {

scene_t load_stl(std::string file_name) {
  std::fstream in(file_name);

  std::cout << "loading stl file \"" << file_name << "\"..." << std::endl;

  // ignore the header
  in.ignore(80);

  uint32_t primitive_count;
  in.read(reinterpret_cast<char*>(&primitive_count), 4);

  std::cout << "primitive count = " << primitive_count << std::endl;

  scene_t scene;
  scene.primitive_vector = scene_t::primitive_vector_t(primitive_count);

  for (int i = 0; i < primitive_count; i++) {
    in.read(reinterpret_cast<char*>(&scene.primitive_vector[i].normal), 12);
    in.read(reinterpret_cast<char*>(&scene.primitive_vector[i].vertex[0]), 12);
    in.read(reinterpret_cast<char*>(&scene.primitive_vector[i].vertex[1]), 12);
    in.read(reinterpret_cast<char*>(&scene.primitive_vector[i].vertex[2]), 12);
    in.ignore(2);
  }

  return scene;
}

}  // namespace ray_trace
}  // namespace spray