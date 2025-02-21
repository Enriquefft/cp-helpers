#include <concepts>
template <std::integral T> auto floor_div(T dividend, T divisor) -> T {
  return dividend / divisor -
         ((dividend ^ divisor) < 0 && dividend % divisor != 0);
}

template <std::integral T> auto ceil_div(T dividend, T divisor) -> T {
  return dividend / divisor +
         ((dividend ^ divisor) > 0 && dividend % divisor != 0);
}

/**
 * @brief Computes the greatest common divisor (GCD) of two integral values.
 *
 * The GCD is calculated using the Euclidean algorithm, which is efficient and
 * ensures that the result is always positive regardless of input sign.
 *
 * @tparam T Integral type (e.g., int, long, unsigned int, etc.).
 * @param a First value of type T.
 * @param b Second value of type T.
 * @return The greatest common divisor of a and b.
 */
template <std::integral T> constexpr T gcd(T a, T b) noexcept {
  // Ensure that the values are positive for signed integers
  a = (a < 0) ? -a : a;
  b = (b < 0) ? -b : b;

  while (b != 0) {
    T temp = b;
    b = a % b;
    a = temp;
  }
  return a;
}

/**
 * @brief Computes the least common multiple (LCM) of two integral values.
 *
 * This function calculates the LCM using the GCD. The formula used is:
 * LCM(a, b) = |a * b| / GCD(a, b).
 *
 * @tparam T Integral type (e.g., int, long, unsigned int, etc.).
 * @param a First value of type T.
 * @param b Second value of type T.
 * @return The least common multiple of a and b.
 */
template <std::integral T> constexpr T lcm(T a, T b) noexcept {
  if (a == 0 || b == 0) {
    return 0;
  }
  return (a / gcd(a, b)) * b;
}
