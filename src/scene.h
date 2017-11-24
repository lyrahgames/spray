#ifndef SPRAY_SCENE_H_
#define SPRAY_SCENE_H_

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "primitive.h"

namespace spray {
namespace ray_tracer {

struct scene {
  std::vector<primitive> primitive_vector;
};

scene load_stl(std::string file_name);

}  // namespace ray_tracer
}  // namespace spray

#endif  // SPRAY_SCENE_H_