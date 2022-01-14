#ifndef FASTFMT_FASTFMT_HPP
#define FASTFMT_FASTFMT_HPP

#include <string>
#include <string_view>

#include <fastfmt/detail/concepts.hpp>
#include <fastfmt/detail/output_arg.hpp>

namespace fastfmt {
  /*
   * integer output argument factories
   */

  template <detail::integer Int>
  constexpr auto bin(Int value) noexcept {
    return detail::int_output_arg<Int, 2>{.value = value};
  }

  template <detail::integer Int>
  constexpr auto oct(Int value) noexcept {
    return detail::int_output_arg<Int, 8>{.value = value};
  }

  template <detail::integer Int>
  constexpr auto dec(Int value) noexcept {
    return detail::int_output_arg<Int, 10>{.value = value};
  }

  template <detail::integer Int>
  constexpr auto hex(Int value) noexcept {
    return detail::int_output_arg<Int, 16>{.value = value};
  }

  /*
   * floating-point output argument factories
   */

  template <detail::floating_point Float>
  constexpr auto general(Float value, unsigned precision = 0) {
    return detail::float_output_arg<Float>{
        .value = value, .format = std::chars_format::general, .precision = precision};
  }

  template <detail::floating_point Float>
  constexpr auto fixed(Float value, unsigned precision = 0) {
    return detail::float_output_arg<Float>{.value = value, .format = std::chars_format::fixed, .precision = precision};
  }

  template <detail::floating_point Float>
  constexpr auto scientific(Float value, unsigned precision = 0) {
    return detail::float_output_arg<Float>{
        .value = value, .format = std::chars_format::scientific, .precision = precision};
  }

  /*
   * formatting output buffer
   */

  class output {
  public:
    [[nodiscard]] std::string_view str() const noexcept;

    // format characters and strings

    output& operator<<(char c);
    output& operator<<(const char* c_str);
    output& operator<<(std::string_view sv);

    // format boolean

    output& operator<<(bool b);

    // format integers

    template <detail::integer Int>
    output& operator<<(Int i) {
      return *this << dec(i);
    }

    // format floating point

    template <detail::floating_point Float>
    output& operator<<(Float f) {
      return *this << general(f);
    }

    // format output args

    template <detail::output_arg OutputArg>
    output& operator<<(const OutputArg& arg) {
      arg.append_to(buffer_);
      return *this;
    }

  private:
    std::string buffer_;
  };
}

#endif  // FASTFMT_FASTFMT_HPP
