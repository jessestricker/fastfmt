#ifndef FASTFMT_DETAIL_APPEND_TO_STR_HPP
#define FASTFMT_DETAIL_APPEND_TO_STR_HPP

#include <charconv>
#include <climits>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <utility>

#include <fastfmt/detail/checked_arith.hpp>
#include <fastfmt/detail/concepts.hpp>

namespace fastfmt::detail {
  template <std::size_t InitBufSize, std::size_t MaxBufSize, class Value, class... Args>
  void append_to_str(std::string& str, Value&& value, Args&&... args) {
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

  template <integer Int, unsigned base>
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
}

#endif  // FASTFMT_DETAIL_APPEND_TO_STR_HPP
