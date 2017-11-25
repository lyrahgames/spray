#ifndef SPRAY_CAMERA_H_
#define SPRAY_CAMERA_H_

#include <Eigen/Dense>
#include <cassert>
#include <cmath>
#include "ray.h"

namespace spray {
namespace ray_tracer {

class camera {
  using Vector3f = Eigen::Vector3f;

 public:
  const Vector3f& position() const { return position_; }
  const Vector3f& direction() const { return direction_; }
  const Vector3f& up() const { return up_; }
  const Vector3f& right() const { return right_; }
  float field_of_view() const { return field_of_view_; }
  int pixel_rows() const { return pixel_rows_; }
  int pixel_cols() const { return pixel_cols_; }
  int screen_width() const { return pixel_cols_; }
  int screen_height() const { return pixel_rows_; }
  float pixel_size() const { return pixel_size_; }
  float aspect_ratio() const { return aspect_ratio_; }

  void look_at(const Vector3f& eye, const Vector3f& center, const Vector3f& up);
  void set_screen_resolution(int width, int height);
  void set_field_of_view(float fov);

  ray primary_ray(int col, int row) const;

 private:
  void compute_pixel_size();
  void compute_aspect_ratio();

 private:
  Vector3f position_;
  Vector3f direction_;
  Vector3f up_;
  Vector3f right_;
  float field_of_view_;
  int pixel_rows_;
  int pixel_cols_;
  float pixel_size_;
  float aspect_ratio_;
};

}  // namespace ray_tracer
}  // namespace spray

#endif  // SPRAY_CAMERA_H_