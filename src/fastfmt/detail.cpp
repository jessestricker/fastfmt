#include "detail.hpp"

namespace fastfmt::detail {
  void ensure(bool cond, const char *desc) noexcept {
    if (cond) [[likely]] {
      return;
    }
    std::fputs("[fastfmt] implementation bug: ", stderr);
    std::fputs(desc, stderr);
    std::fputs("\n", stderr);
    std::abort();
  }
}
