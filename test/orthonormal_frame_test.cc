#define BOOST_TEST_MODULE orthonormal_frame_test
#include <boost/test/unit_test.hpp>
#include <vector>

#include <spray/ray_tracer/orthonormal_frame.h>
using spray::ray_tracer::Orthonormal_frame;

inline void check_orthonormal_frame_properties(const Orthonormal_frame& frame) {
  // negative directions
  BOOST_CHECK_EQUAL(frame.right(), -frame.left());
  BOOST_CHECK_EQUAL(frame.up(), -frame.down());
  BOOST_CHECK_EQUAL(frame.back(), -frame.front());

  // orthogonal directions
  BOOST_CHECK_SMALL(frame.right().dot(frame.up()), 1e-4f);
  BOOST_CHECK_SMALL(frame.right().dot(frame.back()), 1e-4f);
  BOOST_CHECK_SMALL(frame.up().dot(frame.right()), 1e-4f);
  BOOST_CHECK_SMALL(frame.up().dot(frame.back()), 1e-4f);
  BOOST_CHECK_SMALL(frame.back().dot(frame.right()), 1e-4f);
  BOOST_CHECK_SMALL(frame.back().dot(frame.up()), 1e-4f);

  // normalized directions
  BOOST_CHECK_CLOSE(frame.right().norm(), 1.0f, 1e-4f);
  BOOST_CHECK_CLOSE(frame.back().norm(), 1.0f, 1e-4f);
  BOOST_CHECK_CLOSE(frame.up().norm(), 1.0f, 1e-4f);
}

BOOST_AUTO_TEST_CASE(orthonormal_frame_properties) {
  std::vector<Orthonormal_frame> frames;
  frames.push_back({});
  frames.push_back({{}, {1, 1, 1}, {-2, 2, 2}});

  for (const auto& frame : frames) {
    check_orthonormal_frame_properties(frame);
  }
}