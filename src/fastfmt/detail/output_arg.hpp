#ifndef FASTFMT_DETAIL_OUTPUT_ARG_HPP
#define FASTFMT_DETAIL_OUTPUT_ARG_HPP

#include <charconv>
#include <climits>
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

  template <std::size_t InitBufSize, std::size_t MaxBufSize, class Value, class... Args>
  void append_to_chars(std::string& str, Value&& value, Args&&... args) {
    const auto original_str_size = str.size();
    auto buf_size = InitBufSize;

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
      if (buf_size > MaxBufSize) {
        // prevent too large buffer sizes
        throw std::length_error{"[fastfmt] max. buffer size for number conversions reached"};
      }
    } while (true);
  }

  /*
   * output arg subclasses
   */

  template <integer Int, unsigned base>
  struct int_output_arg : output_arg_base {
    Int value{};

    void append_to(std::string& str) const {
      // initial and maximum buffer size are the same,
      // because the maximum buffer size is known at compile time
      append_to_chars<int_to_chars_max_len(), int_to_chars_max_len()>(str, value, base);
    }

    static consteval auto int_to_chars_max_len() noexcept {
      constexpr auto is_signed = signed_integer<Int>;
      constexpr auto bits = sizeof(Int) * CHAR_BIT;
      if (!is_signed) {
        if (bits == 8) {
          if (base == 2) return 8;
          if (base == 8) return 3;
          if (base == 10) return 3;
          if (base == 16) return 2;
        }
        if (bits == 16) {
          if (base == 2) return 16;
          if (base == 8) return 6;
          if (base == 10) return 5;
          if (base == 16) return 4;
        }
        if (bits == 32) {
          if (base == 2) return 32;
          if (base == 8) return 11;
          if (base == 10) return 10;
          if (base == 16) return 8;
        }
        if (bits == 64) {
          if (base == 2) return 64;
          if (base == 8) return 22;
          if (base == 10) return 20;
          if (base == 16) return 16;
        }
      }
      if (is_signed) {
        if (bits == 8) {
          if (base == 2) return 9;
          if (base == 8) return 4;
          if (base == 10) return 4;
          if (base == 16) return 3;
        }
        if (bits == 16) {
          if (base == 2) return 17;
          if (base == 8) return 7;
          if (base == 10) return 6;
          if (base == 16) return 5;
        }
        if (bits == 32) {
          if (base == 2) return 33;
          if (base == 8) return 12;
          if (base == 10) return 11;
          if (base == 16) return 9;
        }
        if (bits == 64) {
          if (base == 2) return 65;
          if (base == 8) return 23;
          if (base == 10) return 20;
          if (base == 16) return 17;
        }
      }
      throw std::invalid_argument{"unsupported type"};
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
        append_to_chars<init_buf_size, max_buf_size>(str, value, format, precision);
      } else {
        append_to_chars<init_buf_size, max_buf_size>(str, value, format);
      }
    }
  };

  static_assert(output_arg<float_output_arg<double>>);
}

#endif  // FASTFMT_DETAIL_OUTPUT_ARG_HPP
