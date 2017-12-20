#ifndef SPRAY_RAY_TRACER_RAY_H_
#define SPRAY_RAY_TRACER_RAY_H_

#include <Eigen/Dense>

namespace spray {
namespace ray_tracer {

struct ray {
  Eigen::Vector3f origin;
  Eigen::Vector3f direction;

  Eigen::Vector3f operator()(float t) { return origin + t * direction; }
};

struct cached_ray : public ray {
  explicit cached_ray(const ray& r)
      : ray(r),
        inverse_direction(static_cast<Eigen::Vector3f>(
            static_cast<Eigen::Array3f>(r.direction).inverse())),
        direction_is_negative(Eigen::Vector3i(r.direction(0) < 0.0f,
                                              r.direction(1) < 0.0f,
                                              r.direction(2) < 0.0f)) {}

  Eigen::Vector3f inverse_direction;
  Eigen::Vector3i direction_is_negative;
};

}  // namespace ray_tracer
}  // namespace spray

#endif  // SPRAY_RAY_TRACER_RAY_H_