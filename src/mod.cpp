#include <cstdint>
#include <iostream>
#include <stdexcept>

// Modular Integer class for performing arithmetic modulo MOD.
template <int MOD> class ModInt {
public:
  // The value of the modular integer, always in [0, MOD).
  int val{};

  // Default constructor initializes to 0.
  explicit ModInt(int64_t v = 0) { set_value(v); }

  // Constructors for various integer types.
  explicit ModInt(int v) { set_value(static_cast<int64_t>(v)); }
  explicit ModInt(unsigned v) { set_value(static_cast<uint64_t>(v)); }
  explicit ModInt(uint64_t v) { set_value(v); }

  ModInt &operator=(const int &v) { set_value(static_cast<int64_t>(v)); }
  ModInt &operator=(int &&v) { set_value(static_cast<int64_t>(v)); }

  // Sets the value, ensuring it is within [0, MOD).
  void set_value(int64_t v) {
    v = mod_impl(v);
    if (v < 0) {
      v += MOD;
    }
    val = static_cast<int>(v);
  }

  // Explicit conversion operators.
  explicit operator int() const { return val; }
  explicit operator int64_t() const { return val; }

  // Addition assignment operator.
  ModInt &operator+=(const ModInt &other) {
    val += other.val;
    if (val >= MOD) {
      val -= MOD;
    }
    return *this;
  }

  // Subtraction assignment operator.
  ModInt &operator-=(const ModInt &other) {
    val -= other.val;
    if (val < 0) {
      val += MOD;
    }
    return *this;
  }

  // Multiplication assignment operator.
  ModInt &operator*=(const ModInt &other) {
    val = static_cast<int>(mod_impl(static_cast<int64_t>(val) * other.val));
    return *this;
  }

  // Division assignment operator.
  ModInt &operator/=(const ModInt &other) { return *this *= other.inverse(); }

  // Modular inverse.
  [[nodiscard]] ModInt inverse() const {
    int a = val, m = MOD;
    int u = 1, v = 0;
    while (m != 0) {
      int t = a / m;
      a -= t * m;
      std::swap(a, m);
      u -= t * v;
      std::swap(u, v);
    }
    if (a != 1) {
      throw std::runtime_error("Modular inverse does not exist");
    }
    return ModInt(u);
  }

  // Modular exponentiation.
  [[nodiscard]] ModInt pow(int64_t exp) const {
    ModInt result(1);
    ModInt base(val);
    if (exp < 0) {
      base = base.inverse();
      exp = -exp;
    }
    while (exp > 0) {
      if ((exp & 1) != 0) {
        result *= base;
      }
      base *= base;
      exp >>= 1;
    }
    return result;
  }

  // Unary minus operator.
  ModInt operator-() const { return ModInt(-val); }

  // Equality operators.
  friend bool operator==(const ModInt &lhs, const ModInt &rhs) {
    return lhs.val == rhs.val;
  }

  friend bool operator!=(const ModInt &lhs, const ModInt &rhs) {
    return lhs.val != rhs.val;
  }

  // Stream output operator.
  friend std::ostream &operator<<(std::ostream &os, const ModInt &m) {
    return os << m.val;
  }

  // Increment and decrement operators.
  ModInt &operator++() { return *this += 1; }
  ModInt &operator--() { return *this -= 1; }
  ModInt operator++(int) {
    ModInt temp = *this;
    ++*this;
    return temp;
  }
  ModInt operator--(int) {
    ModInt temp = *this;
    --*this;
    return temp;
  }

private:
  // Helper function to compute the modulo operation.
  static int64_t mod_impl(int64_t x) {
#if defined(USE_FAST_MOD)
    return fast_mod(x, MOD);
#else
    return x % MOD;
#endif
  }

  // Fast modulo function for specific platforms.
  static uint64_t fast_mod(uint64_t x, uint64_t m) {
#if defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
    // Use compiler intrinsics or inline assembly if available.
    uint64_t result = 0;
    __asm__("divq %4" : "=a"(result), "=d"(x) : "d"(0), "a"(x), "r"(m));
    return x;
#else
    return x % m;
#endif
  }
};

// Binary arithmetic operators.
template <int MOD>
ModInt<MOD> operator+(ModInt<MOD> lhs, const ModInt<MOD> &rhs) {
  lhs += rhs;
  return lhs;
}

template <int MOD>
ModInt<MOD> operator-(ModInt<MOD> lhs, const ModInt<MOD> &rhs) {
  lhs -= rhs;
  return lhs;
}

template <int MOD>
ModInt<MOD> operator*(ModInt<MOD> lhs, const ModInt<MOD> &rhs) {
  lhs *= rhs;
  return lhs;
}

template <int MOD>
ModInt<MOD> operator/(ModInt<MOD> lhs, const ModInt<MOD> &rhs) {
  lhs /= rhs;
  return lhs;
}

// Usage examples.
const int MOD = 998244353;
using mod_int = ModInt<MOD>;

int main() {
  mod_int a = 10;
  mod_int b = 3;
  mod_int c = a + b;
  mod_int d = a * b;
  mod_int e = a / b;
  mod_int f = a.pow(5);
  std::cout << "a + b = " << c << "\n";
  std::cout << "a * b = " << d << "\n";
  std::cout << "a / b = " << e << "\n";
  std::cout << "a ^ 5 = " << f << "\n";
  return 0;
}
