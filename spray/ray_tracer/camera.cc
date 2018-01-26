#include "camera.h"

namespace spray {
namespace ray_tracer {

float Camera::vertical_field_of_view() const {
  // return 2.0 * atanf(tanf(field_of_view_ * 0.5f) / aspect_ratio_);
  return field_of_view_;
}

float Camera::horizontal_field_of_view() const {
  return 2.0 * std::atan(std::tan(field_of_view_ * 0.5f) * aspect_ratio_);
}

float Camera::opengl_field_of_view() const {
  return field_of_view_ * 180 / M_PI;
}

void Camera::look_at(const Eigen::Vector3f& eye, const Eigen::Vector3f& center,
                     const Eigen::Vector3f& up) {
  frame_ = Orthonormal_frame(eye, eye - center, up);
}

void Camera::set_screen_resolution(int width, int height) {
  screen_height_ = height;
  screen_width_ = width;
  compute_pixel_size();
  compute_aspect_ratio();
}

void Camera::set_field_of_view(float fov) {
  if (fov <= 0.0f || fov >= static_cast<float>(M_PI)) return;
  field_of_view_ = fov;
  compute_pixel_size();
}

void Camera::set_horizontal_field_of_view(float fov) {
  if (fov <= 0.0f || fov >= static_cast<float>(M_PI)) return;
  const float vfov = 2.0 * std::atan(std::tan(fov * 0.5f) / aspect_ratio_);
  set_field_of_view(vfov);
}

void Camera::compute_pixel_size() {
  // pixel_size_ =
  //     2.0f * tanf(field_of_view_ * 0.5f) / static_cast<float>(screen_width_);
  pixel_size_ = 2.0f * std::tan(field_of_view_ * 0.5f) /
                static_cast<float>(screen_height_);
}

void Camera::compute_aspect_ratio() {
  aspect_ratio_ =
      static_cast<float>(screen_width_) / static_cast<float>(screen_height_);
}

// ray primary_ray(const Camera& cam, int col, int row) {
//   ray r{cam.position(),
//         cam.direction() +
//             cam.pixel_size() *
//                 ((static_cast<float>(col) -
//                   0.5f * static_cast<float>(cam.screen_width()) + 0.5f) *
//                      cam.frame().right() +
//                  (static_cast<float>(row) -
//                   0.5f * static_cast<float>(cam.screen_height()) + 0.5f) *
//                      cam.frame().up())};

//   r.direction.normalize();
//   return r;
// }

Ray primary_ray(const Camera& camera, int col, int row) {
  Ray ray(camera.position(),
          camera.direction() +
              camera.pixel_size() *
                  ((static_cast<float>(col) -
                    0.5f * static_cast<float>(camera.screen_width()) + 0.5f) *
                       camera.frame().right() +
                   (static_cast<float>(row) -
                    0.5f * static_cast<float>(camera.screen_height()) + 0.5f) *
                       camera.frame().up()));
  ray.direction().normalize();
  return ray;
}

// ray jittered_primary_ray(const Camera& cam, int col, int row,
//                          std::mt19937& rng) {
//   std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
//   ray r{cam.position(),
//         cam.direction() + cam.pixel_size() *
//                               ((static_cast<float>(col) -
//                                 0.5f * static_cast<float>(cam.screen_width())
//                                 + distribution(rng)) *
//                                    cam.frame().right() +
//                                (static_cast<float>(row) -
//                                 0.5f *
//                                 static_cast<float>(cam.screen_height()) +
//                                 distribution(rng)) *
//                                    cam.frame().up())};
//   r.direction.normalize();
//   return r;
// }

Ray jittered_primary_ray(const Camera& camera, int col, int row,
                         std::mt19937& rng) {
  std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
  Ray ray(camera.position(),
          camera.direction() +
              camera.pixel_size() *
                  ((static_cast<float>(col) -
                    0.5f * static_cast<float>(camera.screen_width()) +
                    distribution(rng)) *
                       camera.frame().right() +
                   (static_cast<float>(row) -
                    0.5f * static_cast<float>(camera.screen_height()) +
                    distribution(rng)) *
                       camera.frame().up()));
  ray.direction().normalize();
  return ray;
}

}  // namespace ray_tracer
}  // namespace spray