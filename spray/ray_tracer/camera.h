#ifndef SPRAY_RAY_TRACER_CAMERA_H_
#define SPRAY_RAY_TRACER_CAMERA_H_

#include <Eigen/Dense>
#include <cassert>
#include <cmath>
#include <random>
#include "orthonormal_frame.h"
#include "ray.h"

namespace spray {
namespace ray_tracer {

class Camera {
  using Vector3f = Eigen::Vector3f;

 public:
  const Vector3f& position() const { return frame_.origin(); }
  Vector3f direction() const { return -frame_.back(); }
  const Orthonormal_frame& frame() const { return frame_; }
  float field_of_view() const { return field_of_view_; }
  float vertical_field_of_view() const;
  float horizontal_field_of_view() const;
  float opengl_field_of_view() const;
  int screen_width() const { return screen_width_; }
  int screen_height() const { return screen_height_; }
  float pixel_size() const { return pixel_size_; }
  float aspect_ratio() const { return aspect_ratio_; }

  void look_at(const Vector3f& eye, const Vector3f& center, const Vector3f& up);
  void set_screen_resolution(int width, int height);
  void set_field_of_view(float fov);
  void set_vertical_field_of_view(float fov) { set_field_of_view(fov); }
  void set_horizontal_field_of_view(float fov);

 private:
  void compute_pixel_size();
  void compute_aspect_ratio();

 private:
  Orthonormal_frame frame_;

  float field_of_view_;
  int screen_height_;
  int screen_width_;
  float pixel_size_;
  float aspect_ratio_;
};

Ray primary_ray(const Camera& camera, int col, int row);
Ray jittered_primary_ray(const Camera& camera, int col, int row,
                         std::mt19937& rng);

}  // namespace ray_tracer
}  // namespace spray

#endif  // SPRAY_RAY_TRACER_CAMERA_H_