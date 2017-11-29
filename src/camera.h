#ifndef SPRAY_CAMERA_H_
#define SPRAY_CAMERA_H_

#include <Eigen/Dense>
#include <cassert>
#include <cmath>
#include "orthonormal_frame.h"
#include "ray.h"

namespace spray {
namespace ray_tracer {

class camera {
  using Vector3f = Eigen::Vector3f;

 public:
  const Vector3f& position() const { return frame_.origin(); }
  Vector3f direction() const { return -frame_.back(); }
  const orthonormal_frame& frame() const { return frame_; }
  float field_of_view() const { return field_of_view_; }
  int screen_width() const { return screen_width_; }
  int screen_height() const { return screen_height_; }
  float pixel_size() const { return pixel_size_; }
  float aspect_ratio() const { return aspect_ratio_; }

  void look_at(const Vector3f& eye, const Vector3f& center, const Vector3f& up);
  void set_screen_resolution(int width, int height);
  void set_field_of_view(float fov);

 private:
  void compute_pixel_size();
  void compute_aspect_ratio();

 private:
  orthonormal_frame frame_;

  float field_of_view_;
  int screen_height_;
  int screen_width_;
  float pixel_size_;
  float aspect_ratio_;
};

ray primary_ray(const camera& cam, int col, int row);

}  // namespace ray_tracer
}  // namespace spray

#endif  // SPRAY_CAMERA_H_