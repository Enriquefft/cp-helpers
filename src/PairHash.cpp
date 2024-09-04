#include <cstddef>
#include <utility>

template <typename T> struct HashFunction {
  auto operator()(const std::pair<T, T> &pair) const -> size_t {
    return pair.first ^ pair.second;
  }
};
