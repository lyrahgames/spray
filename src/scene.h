#ifndef SPRAY_SCENE_H_
#define SPRAY_SCENE_H_

#include <vector>
#include "primitive.h"

namespace spray {
namespace ray_trace {

struct scene_t {
  using primitive_vector_t = std::vector<primitive_t>;

  primitive_vector_t primitive_vector;
};

}  // namespace ray_trace
}  // namespace spray

#endif  // SPRAY_SCENE_H_