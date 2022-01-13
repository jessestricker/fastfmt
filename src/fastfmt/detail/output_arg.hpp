#ifndef FASTFMT_DETAIL_OUTPUT_ARG_HPP
#define FASTFMT_DETAIL_OUTPUT_ARG_HPP

#include <charconv>
#include <concepts>
#include <cstddef>
#include <stdexcept>
#include <string>

#include <fastfmt/detail/checked_arith.hpp>
#include <fastfmt/detail/concepts.hpp>

namespace fastfmt::detail {
  /*
   * output arg base & concept
   */
  struct output_arg_base {};

  template <class T>
  concept output_arg = std::derived_from<T, output_arg_base> && requires(const T& arg, std::string& str) {
    { arg.append_to(str) } -> std::same_as<void>;
  };

  /*
   * appending numbers to string using std::to_chars
   */

  template <class Value, class... Args>
  void append_to_chars(std::string& str, Value&& value, Args&&... args) {
    static constexpr auto initial_buf_size = std::size_t{128};
    static constexpr auto max_buf_size = std::size_t{1024};

    const auto original_str_size = str.size();
    auto buf_size = initial_buf_size;

    do {
      // grow string to make place for buffer
      const auto str_size = checked_add(original_str_size, buf_size);
      str.resize(str_size);
      const auto buf_begin = str.data() + original_str_size;
      const auto buf_end = str.data() + str.size();

      // write number into buffer
      const auto [num_end, ec] =
          std::to_chars(buf_begin, buf_end, std::forward<Value>(value), std::forward<Args>(args)...);
      if (ec == std::errc{}) {
        // trim buffer to end of number
        str.resize(num_end - str.data());
        return;
      }

      // increase buffer size and try again
      buf_size = checked_mul(buf_size, std::size_t{2});
      if (buf_size > max_buf_size) {
        // prevent too large buffer sizes
        throw std::length_error{"[fastfmt] max. buffer size for number conversions reached"};
      }
    } while (true);
  }

  /*
   * output arg subclasses
   */

  template <integer Int>
  struct int_output_arg : output_arg_base {
    Int value{};
    unsigned base{};

    void append_to(std::string& str) const {
      append_to_chars(str, value, base);
    }
  };

  static_assert(output_arg<int_output_arg<int>>);

  template <floating_point Float>
  struct float_output_arg : output_arg_base {
    Float value{};
    std::chars_format format{};
    unsigned precision{};  ///< optional, may be zero

    void append_to(std::string& str) const {
      if (precision != 0) {
        append_to_chars(str, value, format, precision);
      } else {
        append_to_chars(str, value, format);
      }
    }
  };

  static_assert(output_arg<float_output_arg<double>>);
}

#endif  // FASTFMT_DETAIL_OUTPUT_ARG_HPP
