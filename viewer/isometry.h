#ifndef SPRAY_VIEWER_ISOMETRY_H_
#define SPRAY_VIEWER_ISOMETRY_H_

#include <stdexcept>

#include <Eigen/Geometry>

#include "permutation.h"

using Isometry_base =
    Eigen::Transform<float, 3, Eigen::AffineCompact, Eigen::RowMajor>;

class Isometry : public Isometry_base {
 public:
  template <int I, int J, int Orientation>
  struct Construction_system {
    static constexpr int i = I;
    static constexpr int j = J;
    static constexpr int k = 3 - I - J;
    static constexpr int sign = Orientation;
  };

  using Positive_zy_construction = Construction_system<2, 1, 1>;

  Isometry() : Isometry_base{Isometry_base::Identity()} {}

  template <class System = Positive_zy_construction>
  Isometry(const Eigen::Vector3f& origin, const Eigen::Vector3f& v0,
           const Eigen::Vector3f& v1, System s = Positive_zy_construction{}) {
    // use Gram-Schmidt process to orthonormalize given vectors
    matrix().col(System::i) = v0;
    matrix().col(System::i).normalize();

    matrix().col(System::j) =
        v1 - matrix().col(System::i).dot(v1) * matrix().col(System::i);

    constexpr float epsilon = 1e-6f;
    if (matrix().col(System::j).norm() <= epsilon)
      throw std::invalid_argument(
          "Cannot constuct isometry! Given vectors are linear dependent!");

    matrix().col(System::j).normalize();

    matrix().col(System::k) =
        System::sign *
        signum(Permutation<System::i + 1, System::j + 1, System::k + 1>{}) *
        matrix().col(System::i).cross(matrix().col(System::j));
    matrix().col(System::k).normalize();

    matrix().col(3) = origin;
  }

  Eigen::Vector3f origin() const { return matrix().col(3); }
  Eigen::Vector3f basis_x() const { return matrix().col(0); }
  Eigen::Vector3f basis_z() const { return matrix().col(2); }
  Eigen::Vector3f basis_y() const { return matrix().col(1); }
};

#endif  // SPRAY_VIEWER_ISOMETRY_H_