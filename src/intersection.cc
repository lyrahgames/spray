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

bool intersect(const ray& r, const aabb& box) {
  const Eigen::Array3f inverse_direction =
      static_cast<Eigen::Array3f>(r.direction).inverse();
  const Eigen::Array3f t1_vector =
      static_cast<Eigen::Array3f>(box.min - r.origin) * inverse_direction;
  const Eigen::Array3f t2_vector =
      static_cast<Eigen::Array3f>(box.max - r.origin) * inverse_direction;
  const Eigen::Vector3f t_min_vector =
      static_cast<Eigen::Vector3f>(t1_vector.min(t2_vector));
  const Eigen::Vector3f t_max_vector =
      static_cast<Eigen::Vector3f>(t1_vector.max(t2_vector));

  return t_min_vector.maxCoeff() <= t_max_vector.minCoeff();
}

bool intersect(const ray& r, const aabb& box, float& t) {
  const Eigen::Array3f inverse_direction =
      static_cast<Eigen::Array3f>(r.direction).inverse();
  const Eigen::Array3f t1_vector =
      static_cast<Eigen::Array3f>(box.min - r.origin) * inverse_direction;
  const Eigen::Array3f t2_vector =
      static_cast<Eigen::Array3f>(box.max - r.origin) * inverse_direction;
  const Eigen::Vector3f t_min_vector =
      static_cast<Eigen::Vector3f>(t1_vector.min(t2_vector));
  const Eigen::Vector3f t_max_vector =
      static_cast<Eigen::Vector3f>(t1_vector.max(t2_vector));

  t = t_min_vector.maxCoeff();

  return t <= t_max_vector.minCoeff();
}

}  // namespace ray_tracer
}  // namespace spray