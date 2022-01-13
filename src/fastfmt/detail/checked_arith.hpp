#ifndef FASTFMT_DETAIL_CHECKED_ARITH_HPP
#define FASTFMT_DETAIL_CHECKED_ARITH_HPP

#include <concepts>
#include <limits>
#include <stdexcept>

namespace fastfmt::detail {
  template <std::unsigned_integral T>
  constexpr T checked_add(T lhs, T rhs) {
    if ((std::numeric_limits<T>::max() - lhs) < rhs) [[unlikely]] {
      throw std::domain_error{"[fastfmt] unsigned integer wrap in addition"};
    }
    return lhs + rhs;
  }

  template <std::unsigned_integral T>
  constexpr T checked_mul(T lhs, T rhs) {
    if ((std::numeric_limits<T>::max() / lhs) < rhs) [[unlikely]] {
      throw std::domain_error{"[fastfmt] unsigned integer wrap in multiplication"};
    }
    return lhs * rhs;
  }
}

#endif  // FASTFMT_DETAIL_CHECKED_ARITH_HPP
