#include <concepts>
template <std::integral T> auto floor_div(T dividend, T divisor) -> T {
  return dividend / divisor -
         ((dividend ^ divisor) < 0 && dividend % divisor != 0);
}

template <std::integral T> auto ceil_div(T dividend, T divisor) -> T {
  return dividend / divisor +
         ((dividend ^ divisor) > 0 && dividend % divisor != 0);
}
