#include "detail.hpp"

namespace fastfmt::detail {
  void ensure(bool cond, const char* desc, const char* file, int line) noexcept {
    if (cond) [[likely]] {
      return;
    }
    static constexpr auto format = "[fastfmt] implementation bug: %s, in %s:%i\n";
    std::fprintf(stderr, format, desc, file, line);
    std::abort();
  }
}
