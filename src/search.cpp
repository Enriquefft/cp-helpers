#include <cstdint>
#include <iostream>
#include <optional>
#include <vector>

template <typename T>
concept numeric = std::is_arithmetic_v<T>;

template <numeric T>
inline auto binary_search(const std::vector<T> &v, T x) -> std::optional<T> {
  T left = 0, right = v.size();

  while (left < right) {
    T middle = (left + right) / 2;
    if (v.at(middle) == x) {
      return middle;
    }
    if (v.at(middle) < x) {
      left = middle + 1;
    } else {
      right = middle;
    }
  }
  return std::nullopt;
}
