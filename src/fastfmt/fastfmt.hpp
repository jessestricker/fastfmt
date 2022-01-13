#ifndef FASTFMT_FASTFMT_HPP
#define FASTFMT_FASTFMT_HPP

#include <string>
#include <string_view>

#include <fastfmt/detail.hpp>

namespace fastfmt {
  template <detail::integer Int>
  constexpr auto bin(Int value) noexcept {
    return detail::integer_arg<Int, 2>{value};
  }

  template <detail::integer Int>
  constexpr auto oct(Int value) noexcept {
    return detail::integer_arg<Int, 8>{value};
  }

  template <detail::integer Int>
  constexpr auto dec(Int value) noexcept {
    return detail::integer_arg<Int, 10>{value};
  }

  template <detail::integer Int>
  constexpr auto hex(Int value) noexcept {
    return detail::integer_arg<Int, 16>{value};
  }

  template <detail::floating_point Float>
  constexpr auto fixed(Float value, unsigned precision = 0) {
    return detail::float_arg<Float>{value, std::chars_format::fixed, precision};
  }

  template <detail::floating_point Float>
  constexpr auto scientific(Float value, unsigned precision = 0) {
    return detail::float_arg<Float>{value, std::chars_format::scientific, precision};
  }

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
      return *this << detail::float_arg{f, std::chars_format::general, 0};
    }

    // format appendable args (see detail.hpp)

    template <detail::appendable Appendable>
    output& operator<<(Appendable appendable) {
      appendable.append_to(buffer_);
      return *this;
    }

  private:
    std::string buffer_;
  };
}

#endif  // FASTFMT_FASTFMT_HPP
