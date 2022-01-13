#include <cstdint>

#include <catch2/catch_test_macros.hpp>
#include <fastfmt/fastfmt.hpp>

namespace ff = fastfmt;
using namespace std::string_view_literals;
template <class T>
using lim = std::numeric_limits<T>;

#define FASTFMT_CHECK(value__, expected__) \
  do {                                     \
    auto out = ff::output{};               \
    out << (value__);                      \
    CHECK(out.str() == (expected__));      \
  } while (false)

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

TEST_CASE("formatting bool") {
  FASTFMT_CHECK(true, "true");
  FASTFMT_CHECK(false, "false");
}

TEST_CASE("formatting unsigned integer, base 10") {
  FASTFMT_CHECK(lim<std::uint8_t>::min(), "0");
  FASTFMT_CHECK(lim<std::uint8_t>::max(), "255");
  FASTFMT_CHECK(lim<std::uint16_t>::min(), "0");
  FASTFMT_CHECK(lim<std::uint16_t>::max(), "65535");
  FASTFMT_CHECK(lim<std::uint32_t>::min(), "0");
  FASTFMT_CHECK(lim<std::uint32_t>::max(), "4294967295");
  FASTFMT_CHECK(lim<std::uint64_t>::min(), "0");
  FASTFMT_CHECK(lim<std::uint64_t>::max(), "18446744073709551615");
}

TEST_CASE("formatting signed integer, base 10") {
  FASTFMT_CHECK(lim<std::int8_t>::min(), "-128");
  FASTFMT_CHECK(lim<std::int8_t>::max(), "127");
  FASTFMT_CHECK(lim<std::int16_t>::min(), "-32768");
  FASTFMT_CHECK(lim<std::int16_t>::max(), "32767");
  FASTFMT_CHECK(lim<std::int32_t>::min(), "-2147483648");
  FASTFMT_CHECK(lim<std::int32_t>::max(), "2147483647");
  FASTFMT_CHECK(lim<std::int64_t>::min(), "-9223372036854775808");
  FASTFMT_CHECK(lim<std::int64_t>::max(), "9223372036854775807");
}

TEST_CASE("formatting integer, base 2") {
  FASTFMT_CHECK(ff::bin(lim<std::uint8_t>::min()), "0");
  FASTFMT_CHECK(ff::bin(lim<std::uint8_t>::max()), "11111111");
  FASTFMT_CHECK(ff::bin(lim<std::int8_t>::min()), "-10000000");
  FASTFMT_CHECK(ff::bin(lim<std::int8_t>::max()), "1111111");
}

TEST_CASE("formatting integer, base 8") {
  FASTFMT_CHECK(ff::oct(lim<std::uint8_t>::min()), "0");
  FASTFMT_CHECK(ff::oct(lim<std::uint8_t>::max()), "377");
  FASTFMT_CHECK(ff::oct(lim<std::int8_t>::min()), "-200");
  FASTFMT_CHECK(ff::oct(lim<std::int8_t>::max()), "177");
}

TEST_CASE("formatting integer, base 16") {
  FASTFMT_CHECK(ff::hex(lim<std::uint8_t>::min()), "0");
  FASTFMT_CHECK(ff::hex(lim<std::uint8_t>::max()), "ff");
  FASTFMT_CHECK(ff::hex(lim<std::int8_t>::min()), "-80");
  FASTFMT_CHECK(ff::hex(lim<std::int8_t>::max()), "7f");
}

TEST_CASE("formatting multiple arguments") {
  auto out = ff::output{};
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

    friend ff::output& operator<<(ff::output& out, const person& p) {
      return out << p.name << " (age " << p.age << ")";
    }
  };
}

TEST_CASE("formatting custom type") {
  FASTFMT_CHECK((person{"Alice", 42}), "Alice (age 42)");
  FASTFMT_CHECK((person{"Bob", 49}), "Bob (age 49)");
}
