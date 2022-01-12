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
