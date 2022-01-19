#ifndef FASTFMT_DETAIL_OUTPUT_ARG_HPP
#define FASTFMT_DETAIL_OUTPUT_ARG_HPP

#include <charconv>
#include <concepts>
#include <cstddef>
#include <string>
#include <utility>

#include <fastfmt/detail/append_to_str.hpp>
#include <fastfmt/detail/concepts.hpp>

namespace fastfmt::detail {
  /*
   * output arg base & concept
   */
  struct output_arg_base {};

  template <class T>
  concept output_arg = std::derived_from<T, output_arg_base> && requires(T&& arg, std::string& str) {
    { std::forward<T>(arg).append_to(str) } -> std::same_as<void>;
  };

  /*
   * output arg subclasses
   */

  template <integer Int, unsigned base>
  struct int_output_arg : output_arg_base {
    Int value{};

    void append_to(std::string& str) const {
      // initial and maximum buffer size are the same,
      // because the maximum buffer size is known at compile time
      static constexpr auto buf_size = int_to_chars_max_len<Int, base>();
      append_to_str<buf_size, buf_size>(str, value, base);
    }
  };

  static_assert(output_arg<int_output_arg<int, 10>>);

  template <floating_point Float>
  struct float_output_arg : output_arg_base {
    Float value{};
    std::chars_format format{};
    unsigned precision{};  ///< optional, may be zero

    void append_to(std::string& str) const {
      static constexpr auto init_buf_size = std::size_t{128};
      static constexpr auto max_buf_size = std::size_t{1024};

      if (precision != 0) {
        append_to_str<init_buf_size, max_buf_size>(str, value, format, precision);
      } else {
        append_to_str<init_buf_size, max_buf_size>(str, value, format);
      }
    }
  };

  static_assert(output_arg<float_output_arg<double>>);
}

#endif  // FASTFMT_DETAIL_OUTPUT_ARG_HPP
