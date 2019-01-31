#ifndef SPRAY_RAY_TRACER_MORTON_H_
#define SPRAY_RAY_TRACER_MORTON_H_

#include <cstdint>
#include <utility>
#include <vector>

namespace spray {
namespace morton {

template <typename T>
struct primitive {
  uint32_t code;
  T data;
};

inline uint32_t shift2(uint32_t x) {
  x = (x | (x << 8)) & 0b00110000000011111111;
  x = (x | (x << 4)) & 0b00110000111100001111;
  x = (x | (x << 2)) & 0b00110011001100110011;
  x = (x | (x << 1)) & 0b01010101010101010101;
  return x;
}

inline uint32_t shift3(uint32_t x) {
  x = (x | (x << 16)) & 0b00000011000000000000000011111111;
  x = (x | (x << 8)) & 0b00000011000000001111000000001111;
  x = (x | (x << 4)) & 0b00000011000011000011000011000011;
  x = (x | (x << 2)) & 0b00001001001001001001001001001001;
  return x;
}

inline uint32_t encode2(uint32_t x, uint32_t y) {
  return (shift2(y) << 1) | shift2(x);
}

inline uint32_t encode3(uint32_t x, uint32_t y, uint32_t z) {
  return (shift3(z) << 2) | (shift3(y) << 1) | shift3(x);
}

inline uint32_t encode(uint32_t x, uint32_t y, uint32_t z) {
  return (shift3(z) << 2) | (shift3(y) << 1) | shift3(x);
}

template <typename T>
inline void radix_sort(std::vector<primitive<T>> &v) {
  std::vector<primitive<T>> tmp(v.size());
  constexpr int bits_per_pass = 6;
  constexpr int bit_count = 30;
  constexpr int pass_count = bit_count / bits_per_pass;

  for (int pass = 0; pass < pass_count; ++pass) {
    const int low_bit = pass * bits_per_pass;
    std::vector<primitive<T>> &in = (pass & 1) ? (tmp) : (v);
    std::vector<primitive<T>> &out = (pass & 1) ? (v) : (tmp);

    constexpr int bucket_count = 1 << bits_per_pass;
    int bucket[bucket_count] = {0};
    constexpr int bit_mask = (1 << bits_per_pass) - 1;

    for (const primitive<T> &p : in) {
      const int bucket_index = (p.code >> low_bit) & bit_mask;
      ++bucket[bucket_index];
    }

    int out_index[bucket_count];
    out_index[0] = 0;

    for (int i = 1; i < bucket_count; ++i) {
      out_index[i] = out_index[i - 1] + bucket[i - 1];
    }

    for (const primitive<T> &p : in) {
      const int bucket_index = (p.code >> low_bit) & bit_mask;
      out[out_index[bucket_index]++] = p;
    }
  }

  if (pass_count & 1) {
    std::swap(v, tmp);
  }
}

}  // namespace morton
}  // namespace spray

#endif  // SPRAY_RAY_TRACER_MORTON_H_