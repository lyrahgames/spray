#include "intersection.h"

namespace spray {
namespace ray_tracer {

bool intersect(const ray& r, const primitive& p) {
  const Eigen::Vector3f edge1 = p.vertex[1] - p.vertex[0];
  const Eigen::Vector3f edge2 = p.vertex[2] - p.vertex[0];
  const Eigen::Vector3f p_vector = r.direction.cross(edge2);
  const float determinant = edge1.dot(p_vector);

  if (determinant > -intersection_epsilon && determinant < intersection_epsilon)
    return false;

  const float inverse_determinant = 1.0f / determinant;

  const Eigen::Vector3f t_vector = r.origin - p.vertex[0];
  const float u = t_vector.dot(p_vector) * inverse_determinant;

  if (u < 0.0f || u > 1.0f) return false;

  const Eigen::Vector3f q_vector = t_vector.cross(edge1);
  const float v = r.direction.dot(q_vector) * inverse_determinant;

  if (v < 0.0f || u + v > 1.0f) return false;

  // const float t = edge2.dot(q_vector) * inverse_determinant;

  return true;
}

}  // namespace ray_tracer
}  // namespace spray