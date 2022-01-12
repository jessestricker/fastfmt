#include "fastfmt.hpp"

namespace fastfmt {
  std::string_view output::str() const noexcept {
    return buffer_;
  }

  output& output::operator<<(char c) {
    buffer_.push_back(c);
    return *this;
  }

  output& output::operator<<(const char* c_str) {
    buffer_.append(c_str);
    return *this;
  }

  output& output::operator<<(std::string_view sv) {
    buffer_.append(sv);
    return *this;
  }

  output& output::operator<<(bool b) {
    static constexpr std::string_view true_str = "true";
    static constexpr std::string_view false_str = "false";
    return *this << (b ? true_str : false_str);
  }
}
