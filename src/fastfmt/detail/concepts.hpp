#ifndef FASTFMT_DETAIL_CONCEPTS_HPP
#define FASTFMT_DETAIL_CONCEPTS_HPP

#include <type_traits>

namespace fastfmt::detail {
  template <class T, class... Set>
  concept one_of = (std::is_same_v<std::remove_cv_t<T>, Set> || ...);

  template <class T>
  concept signed_integer = one_of<T, signed char, short, int, long, long long>;

  template <class T>
  concept unsigned_integer = one_of<T, unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long>;

  template <class T>
  concept integer = signed_integer<T> || unsigned_integer<T>;

  template <class T>
  concept floating_point = one_of<T, float, double, long double>;
}

#endif  // FASTFMT_DETAIL_CONCEPTS_HPP
