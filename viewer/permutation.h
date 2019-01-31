#ifndef SPRAY_VIEWER_PERMUTATION_H_
#define SPRAY_VIEWER_PERMUTATION_H_

template <int i, int j, int k>
struct Permutation {
  static constexpr int signum = 0;
};

template <>
struct Permutation<1, 2, 3> {
  static constexpr int signum = 1;
};

template <>
struct Permutation<1, 3, 2> {
  static constexpr int signum = -1;
};

template <>
struct Permutation<2, 1, 3> {
  static constexpr int signum = -1;
};

template <>
struct Permutation<2, 3, 1> {
  static constexpr int signum = 1;
};

template <>
struct Permutation<3, 1, 2> {
  static constexpr int signum = 1;
};

template <>
struct Permutation<3, 2, 1> {
  static constexpr int signum = -1;
};

template <class Permutation>
constexpr int signum(Permutation p) {
  return Permutation::signum;
}

#endif  // SPRAY_VIEWER_PERMUTATION_H_