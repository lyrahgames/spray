#ifndef SPRAY_KERNEL_H_
#define SPRAY_KERNEL_H_

#include "camera.h"
#include "scene.h"

namespace spray {
namespace ray_tracer {

struct kernel {
  scene* s;
};

}  // namespace ray_tracer
}  // namespace spray

#endif  // SPRAY_KERNEL_H_