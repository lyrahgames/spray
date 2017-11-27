#include "intersection.h"

namespace spray {
namespace ray_tracer {

bool intersect(const ray& r, const Eigen::Vector3f& vertex0,
               const Eigen::Vector3f& vertex1, const Eigen::Vector3f& vertex2,
               Eigen::Vector3f& uvt) {
  const Eigen::Vector3f edge1 = vertex1 - vertex0;
  const Eigen::Vector3f edge2 = vertex2 - vertex0;
  const Eigen::Vector3f p_vector = r.direction.cross(edge2);
  const float determinant = edge1.dot(p_vector);

  if (determinant > -intersection_epsilon && determinant < intersection_epsilon)
    return false;

  const float inverse_determinant = 1.0f / determinant;

  const Eigen::Vector3f t_vector = r.origin - vertex0;
  uvt(0) = t_vector.dot(p_vector) * inverse_determinant;

  if (uvt(0) < 0.0f || uvt(0) > 1.0f) return false;

  const Eigen::Vector3f q_vector = t_vector.cross(edge1);
  uvt(1) = r.direction.dot(q_vector) * inverse_determinant;

  if (uvt(1) < 0.0f || uvt(0) + uvt(1) > 1.0f) return false;

  uvt(2) = edge2.dot(q_vector) * inverse_determinant;

  return true;
}

}  // namespace ray_tracer
}  // namespace spray