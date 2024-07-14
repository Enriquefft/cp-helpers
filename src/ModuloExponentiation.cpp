#include <cstdint>
using ll = intmax_t;
using ull = uintmax_t;

constexpr ll MODULO = 1000000007;

// exponent is very large
// modulo is prime (1e9 + 7)
// base is always 2

/// @brief Binary Exponentiation with modulo
/// @param expo exponent
/// @param base base (default: 2)
/// @param modulo modulo
template <bool USE_FERMAT_LITTLE_THEOREM = false>
constexpr auto bin_mod_exp(ll expo, ll base = 2, ll modulo = MODULO) -> ll {

  if constexpr (USE_FERMAT_LITTLE_THEOREM) {
    expo %= (modulo - 1);
  }

  ll res = 1;

  while (expo > 0) {
    if (expo & 1LL) {
      res = (res * base) % modulo;
    }
    base = (base * base) % modulo;
    expo >>= 1LL;
  }
  return res;
}
