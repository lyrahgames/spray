#ifndef SPRAY_SCENE_H_
#define SPRAY_SCENE_H_

#include <Eigen/Dense>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "aabb.h"

namespace spray {
namespace ray_tracer {

struct scene {
  struct vertex {
    Eigen::Vector3f position;
    Eigen::Vector3f normal;
  };

  struct primitive {
    int vertex_id[3];
    Eigen::Vector3f normal;
  };

  std::vector<vertex> vertex_data;
  std::vector<primitive> primitive_data;
};

aabb bounds(const scene& s);

scene load_stl(std::string file_name);

}  // namespace ray_tracer
}  // namespace spray

#endif  // SPRAY_SCENE_H_