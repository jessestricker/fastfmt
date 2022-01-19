#include "fastfmt.hpp"

#include <cstdint>
#include <numbers>

#include <doctest/doctest.h>

using namespace fastfmt;
using namespace std::string_view_literals;

template <class T>
using lim = std::numeric_limits<T>;

#define FASTFMT_CHECK(value__, expected__) \
  [&] {                                    \
    auto out = output{};                   \
    out << (value__);                      \
    CHECK(out.str() == (expected__));      \
  }()

TEST_SUITE("char/string formatting") {
  TEST_CASE("formatting char") {
    FASTFMT_CHECK('a', "a");
    FASTFMT_CHECK(' ', " ");
    FASTFMT_CHECK('\n', "\n");
  }

  TEST_CASE("formatting c-string") {
    FASTFMT_CHECK("", "");
    FASTFMT_CHECK(" ", " ");
    FASTFMT_CHECK("abc", "abc");
  }

  TEST_CASE("formatting string_view") {
    FASTFMT_CHECK(""sv, "");
    FASTFMT_CHECK(" "sv, " ");
    FASTFMT_CHECK("abc"sv, "abc");
  }
}

TEST_CASE("formatting bool") {
  FASTFMT_CHECK(true, "true");
  FASTFMT_CHECK(false, "false");
}

TEST_SUITE("integer formatting") {
  TEST_CASE_TEMPLATE("format integer 0", T, std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t, std::int8_t,
                     std::int16_t, std::int32_t, std::int64_t) {
    FASTFMT_CHECK(bin(T{0}), "0");
    FASTFMT_CHECK(oct(T{0}), "0");
    FASTFMT_CHECK(dec(T{0}), "0");
    FASTFMT_CHECK(hex(T{0}), "0");
  }

  TEST_CASE("format max length integers, uint8") {
    FASTFMT_CHECK(bin(lim<std::uint8_t>::max()), "11111111");
    FASTFMT_CHECK(oct(lim<std::uint8_t>::max()), "377");
    FASTFMT_CHECK(dec(lim<std::uint8_t>::max()), "255");
    FASTFMT_CHECK(hex(lim<std::uint8_t>::max()), "ff");
  }

  TEST_CASE("format max length integers, uint16") {
    FASTFMT_CHECK(bin(lim<std::uint16_t>::max()), "1111111111111111");
    FASTFMT_CHECK(oct(lim<std::uint16_t>::max()), "177777");
    FASTFMT_CHECK(dec(lim<std::uint16_t>::max()), "65535");
    FASTFMT_CHECK(hex(lim<std::uint16_t>::max()), "ffff");
  }

  TEST_CASE("format max length integers, uint32") {
    FASTFMT_CHECK(bin(lim<std::uint32_t>::max()), "11111111111111111111111111111111");
    FASTFMT_CHECK(oct(lim<std::uint32_t>::max()), "37777777777");
    FASTFMT_CHECK(dec(lim<std::uint32_t>::max()), "4294967295");
    FASTFMT_CHECK(hex(lim<std::uint32_t>::max()), "ffffffff");
  }

  TEST_CASE("format max length integers, uint64") {
    FASTFMT_CHECK(bin(lim<std::uint64_t>::max()), "1111111111111111111111111111111111111111111111111111111111111111");
    FASTFMT_CHECK(oct(lim<std::uint64_t>::max()), "1777777777777777777777");
    FASTFMT_CHECK(dec(lim<std::uint64_t>::max()), "18446744073709551615");
    FASTFMT_CHECK(hex(lim<std::uint64_t>::max()), "ffffffffffffffff");
  }

  TEST_CASE("format max length integers, int8") {
    FASTFMT_CHECK(bin(lim<std::int8_t>::min()), "-10000000");
    FASTFMT_CHECK(oct(lim<std::int8_t>::min()), "-200");
    FASTFMT_CHECK(dec(lim<std::int8_t>::min()), "-128");
    FASTFMT_CHECK(hex(lim<std::int8_t>::min()), "-80");
  }

  TEST_CASE("format max length integers, int16") {
    FASTFMT_CHECK(bin(lim<std::int16_t>::min()), "-1000000000000000");
    FASTFMT_CHECK(oct(lim<std::int16_t>::min()), "-100000");
    FASTFMT_CHECK(dec(lim<std::int16_t>::min()), "-32768");
    FASTFMT_CHECK(hex(lim<std::int16_t>::min()), "-8000");
  }

  TEST_CASE("format max length integers, int32") {
    FASTFMT_CHECK(bin(lim<std::int32_t>::min()), "-10000000000000000000000000000000");
    FASTFMT_CHECK(oct(lim<std::int32_t>::min()), "-20000000000");
    FASTFMT_CHECK(dec(lim<std::int32_t>::min()), "-2147483648");
    FASTFMT_CHECK(hex(lim<std::int32_t>::min()), "-80000000");
  }

  TEST_CASE("format max length integers, int64") {
    FASTFMT_CHECK(bin(lim<std::int64_t>::min()), "-1000000000000000000000000000000000000000000000000000000000000000");
    FASTFMT_CHECK(oct(lim<std::int64_t>::min()), "-1000000000000000000000");
    FASTFMT_CHECK(dec(lim<std::int64_t>::min()), "-9223372036854775808");
    FASTFMT_CHECK(hex(lim<std::int64_t>::min()), "-8000000000000000");
  }
}

TEST_SUITE("integer formatting") {
  TEST_CASE_TEMPLATE("formatting floating point, fixed", T, float, double, long double) {
    static constexpr auto pi = std::numbers::pi_v<T>;
    FASTFMT_CHECK(fixed(pi, 2), "3.14");
    FASTFMT_CHECK(fixed(pi, 4), "3.1416");
    FASTFMT_CHECK(fixed(pi, 6), "3.141593");
  }

  TEST_CASE_TEMPLATE("formatting floating point, scientific", T, float, double, long double) {
    static constexpr auto egamma = std::numbers::egamma_v<T>;
    FASTFMT_CHECK(scientific(egamma, 2), "5.77e-01");
    FASTFMT_CHECK(scientific(egamma, 4), "5.7722e-01");
    FASTFMT_CHECK(scientific(egamma, 6), "5.772157e-01");
  }
}

TEST_SUITE("output buffer") {
  TEST_CASE("formatting multiple arguments") {
    auto out = output{};
    REQUIRE(out.str().empty());
    out << "hello ";
    REQUIRE(out.str() == "hello ");
    out << "world!";
    REQUIRE(out.str() == "hello world!");
  }

  namespace {
    struct person {
      std::string name;
      unsigned age{};

      friend output& operator<<(output& out, const person& p) {
        return out << p.name << " (age " << p.age << ")";
      }
    };
  }

  TEST_CASE("formatting custom type") {
    FASTFMT_CHECK((person{"Alice", 42}), "Alice (age 42)");
    FASTFMT_CHECK((person{"Bob", 49}), "Bob (age 49)");
  }
}
