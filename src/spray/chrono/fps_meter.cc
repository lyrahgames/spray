#include "fps_meter.h"

namespace spray {
namespace chrono {

void Fps_meter::measure() {
  const auto current_time = std::chrono::system_clock::now();
  frame_count_++;

  const float time_difference =
      std::chrono::duration<float>(current_time - last_time_).count();

  if (time_difference >= time_bound_) {
    frame_time_ = time_difference / static_cast<float>(frame_count_);
    last_time_ = current_time;
    frame_count_ = 0;

    std::cout << "fps: " << fps() << "\t spf: " << frame_time() << " s"
              << std::endl;
  }
}

}  // namespace chrono
}  // namespace spray