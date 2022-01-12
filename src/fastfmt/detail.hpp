#ifndef FASTFMT_DETAIL_HPP
#define FASTFMT_DETAIL_HPP

#include <charconv>
#include <climits>
#include <concepts>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

namespace fastfmt::detail {
  template <class T, class... Set>
  concept one_of = (std::same_as<std::remove_cv_t<T>, Set> || ...);

  template <class T>
  concept signed_integer = one_of<T, signed char, short, int, long, long long>;

  template <class T>
  concept unsigned_integer = one_of<T, unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long>;

  template <class T>
  concept integer = signed_integer<T> || unsigned_integer<T>;

  void ensure(bool cond, const char* desc) noexcept;

  class appendable_base {
  protected:
    constexpr ~appendable_base() noexcept = default;
  };

  template <class T>
  concept appendable = std::derived_from<T, appendable_base> && requires(std::string& str, T&& obj) {
    { std::forward<T>(obj).append_to(str) } -> std::same_as<void>;
  };

  /**
   * @tparam base the base of the formatted number, one of {2, 8, 10, 16}
   */
  template <integer Int, unsigned base>
  class integer_arg : public appendable_base {
  public:
    constexpr explicit integer_arg(Int value) noexcept : value_{value} {
    }

    void append_to(std::string& str) {
      static constexpr auto buffer_capacity = int_to_chars_max_len();
      const auto str_old_size = str.size();
      str.resize(str_old_size + buffer_capacity);

      const auto buf_first = str.data() + str_old_size;
      const auto buf_last = str.data() + str.size();

      const auto [str_end_ptr, ec] = std::to_chars(buf_first, buf_last, value_, base);
      ensure(ec == std::errc{}, "buffer capacity exceeded");

      const auto str_new_size = str_end_ptr - str.data();
      str.resize(str_new_size);
    }

  private:
    Int value_;

    /**
     * Returns the maximum length of a formatted number created with std::to_chars for a specific integer type and base.
     * The return values are pre-generated with a Python script. If this function is called with unsupported parameter
     * values, it does not compile.
     */
    static consteval std::size_t int_to_chars_max_len() noexcept {
      const auto is_signed = signed_integer<Int>;
      const auto bits = sizeof(Int) * CHAR_BIT;
      if (!is_signed && bits == 8 && base == 2) return 8;
      if (!is_signed && bits == 8 && base == 8) return 3;
      if (!is_signed && bits == 8 && base == 10) return 3;
      if (!is_signed && bits == 8 && base == 16) return 2;
      if (!is_signed && bits == 16 && base == 2) return 16;
      if (!is_signed && bits == 16 && base == 8) return 6;
      if (!is_signed && bits == 16 && base == 10) return 5;
      if (!is_signed && bits == 16 && base == 16) return 4;
      if (!is_signed && bits == 32 && base == 2) return 32;
      if (!is_signed && bits == 32 && base == 8) return 11;
      if (!is_signed && bits == 32 && base == 10) return 10;
      if (!is_signed && bits == 32 && base == 16) return 8;
      if (!is_signed && bits == 64 && base == 2) return 64;
      if (!is_signed && bits == 64 && base == 8) return 22;
      if (!is_signed && bits == 64 && base == 10) return 20;
      if (!is_signed && bits == 64 && base == 16) return 16;
      if (is_signed && bits == 8 && base == 2) return 9;
      if (is_signed && bits == 8 && base == 8) return 4;
      if (is_signed && bits == 8 && base == 10) return 4;
      if (is_signed && bits == 8 && base == 16) return 3;
      if (is_signed && bits == 16 && base == 2) return 17;
      if (is_signed && bits == 16 && base == 8) return 7;
      if (is_signed && bits == 16 && base == 10) return 6;
      if (is_signed && bits == 16 && base == 16) return 5;
      if (is_signed && bits == 32 && base == 2) return 33;
      if (is_signed && bits == 32 && base == 8) return 12;
      if (is_signed && bits == 32 && base == 10) return 11;
      if (is_signed && bits == 32 && base == 16) return 9;
      if (is_signed && bits == 64 && base == 2) return 65;
      if (is_signed && bits == 64 && base == 8) return 23;
      if (is_signed && bits == 64 && base == 10) return 20;
      if (is_signed && bits == 64 && base == 16) return 17;
      throw std::invalid_argument{"unsupported type"};
    }
  };
}

#endif  // FASTFMT_DETAIL_HPP
