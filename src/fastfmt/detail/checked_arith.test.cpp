#include "checked_arith.hpp"

#include <cmath>
#include <cstdint>
#include <limits>

#include <doctest/doctest.h>

using namespace fastfmt::detail;

TEST_SUITE("checked arithmetic") {
  TEST_CASE_TEMPLATE("addition", T, std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t) {
    using limits = std::numeric_limits<T>;
    CHECK(checked_add(T{1}, T{0}) == T{1});
    CHECK(checked_add(T{0}, T{1}) == T{1});

    CHECK(checked_add(T{limits::max() - 1}, T{1}) == limits::max());
    CHECK(checked_add(T{1}, T{limits::max() - 1}) == limits::max());

    CHECK_THROWS_AS(checked_add(limits::max(), T{1}), std::domain_error);
    CHECK_THROWS_AS(checked_add(T{1}, limits::max()), std::domain_error);
  }

  TEST_CASE_TEMPLATE("multiplication", T, std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t) {
    using limits = std::numeric_limits<T>;
    CHECK(checked_mul(T{2}, T{3}) == T{6});
    CHECK(checked_mul(T{3}, T{2}) == T{6});

    static constexpr auto max_sqrt = static_cast<T>((T{1} << (limits::digits / 2)));
    static constexpr auto max_sqrt_m1 = T{max_sqrt - 1};
    CHECK(checked_mul(max_sqrt, max_sqrt_m1) <= limits::max());
    CHECK(checked_mul(max_sqrt_m1, max_sqrt) <= limits::max());

    CHECK_THROWS_AS(checked_mul(max_sqrt, max_sqrt), std::domain_error);
  }
}
