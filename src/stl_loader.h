#ifndef SPRAY_STL_LOADER_H_
#define SPRAY_STL_LOADER_H_

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include "scene.h"

namespace spray {
namespace ray_trace {

// void load_stl(std::string file_name, scene& s);
scene_t load_stl(std::string file_name);

}  // namespace ray_trace
}  // namespace spray

#endif  // SPRAY_STL_LOADER_H_