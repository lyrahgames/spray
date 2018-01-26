#ifndef SPRAY_RAY_TRACER_SCENE_H_
#define SPRAY_RAY_TRACER_SCENE_H_

#include <Eigen/Dense>
#include <string>
#include <vector>
#include "bounding_box.h"

namespace spray {
namespace ray_tracer {

class Scene {
 public:
  struct vertex {
    Eigen::Vector3f position;
    Eigen::Vector3f normal;
  };

  struct primitive {
    int vertex_id[3];
    Eigen::Vector3f normal;
  };

  Scene(const std::string& file_path);
  Scene() = default;
  Scene(const Scene&) = default;
  Scene& operator=(const Scene&) = default;
  Scene(Scene&&) = default;
  Scene& operator=(Scene&&) = default;
  ~Scene() = default;

  const std::vector<vertex>& vertex_data() const { return vertex_data_; }
  std::vector<vertex>& vertex_data() { return vertex_data_; }
  const std::vector<primitive>& primitive_data() const {
    return primitive_data_;
  }
  std::vector<primitive>& primitive_data() { return primitive_data_; }

 private:
  std::vector<vertex> vertex_data_;
  std::vector<primitive> primitive_data_;
};

Bounding_box bounds(const Scene& s);

}  // namespace ray_tracer
}  // namespace spray

#endif  // SPRAY_RAY_TRACER_SCENE_H_