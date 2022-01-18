#include "checked_arith.hpp"

#include <cstdint>
#include <limits>

#include <catch2/catch_template_test_macros.hpp>

template <class T>
using lim = std::numeric_limits<T>;
namespace ffd = fastfmt::detail;

TEMPLATE_TEST_CASE("checked add, valid", "", std::uint8_t) {
  CHECK(ffd::checked_add(TestType{1}, TestType{0}) == TestType{1});
  CHECK(ffd::checked_add(TestType{0}, TestType{1}) == TestType{1});

  CHECK(ffd::checked_add(TestType{lim<TestType>::max() - 1}, TestType{1}) == lim<TestType>::max());
  CHECK(ffd::checked_add(TestType{1}, TestType{lim<TestType>::max() - 1}) == lim<TestType>::max());
}

TEMPLATE_TEST_CASE("checked add, invalid", "", std::uint8_t) {
  CHECK_THROWS_AS(ffd::checked_add(lim<TestType>::max(), TestType{1}), std::domain_error);
  CHECK_THROWS_AS(ffd::checked_add(TestType{1}, lim<TestType>::max()), std::domain_error);

  CHECK_THROWS_AS(ffd::checked_add(lim<TestType>::max(), lim<TestType>::max()), std::domain_error);
}
