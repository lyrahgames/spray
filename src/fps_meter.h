#ifndef SPRAY_FPS_METER_H_
#define SPRAY_FPS_METER_H_

#include <chrono>
#include <iostream>

namespace spray {
namespace chrono {

class fps_meter {
 public:
  fps_meter(float time_bound = 1.0f)
      : last_time_(std::chrono::system_clock::now()),
        frame_count_(0),
        time_bound_(time_bound),
        frame_time_(0.0f) {}

  float fps() const { return 1.0f / frame_time_; }
  float frame_time() const { return frame_time_; }

  void measure();

 private:
  std::chrono::time_point<std::chrono::system_clock> last_time_;
  int frame_count_;
  float time_bound_;
  float frame_time_;
};

}  // namespace chrono
}  // namespace spray

#endif  // SPRAY_FPS_METER_H_