#include <cstdint>

#include <catch2/catch_test_macros.hpp>
#include <fastfmt/fastfmt.hpp>

namespace ff = fastfmt;
using namespace std::string_view_literals;
template <class T>
using lim = std::numeric_limits<T>;

template <class T>
void require_fmt(const T& value, std::string_view expected) {
  auto out = ff::output{};
  out << value;
  REQUIRE(out.str() == expected);
}

TEST_CASE("formatting char") {
  require_fmt('a', "a");
  require_fmt(' ', " ");
  require_fmt('\n', "\n");
}

TEST_CASE("formatting c-string") {
  require_fmt("", "");
  require_fmt(" ", " ");
  require_fmt("abc", "abc");
}

TEST_CASE("formatting string_view") {
  require_fmt(""sv, "");
  require_fmt(" "sv, " ");
  require_fmt("abc"sv, "abc");
}

TEST_CASE("formatting bool") {
  require_fmt(true, "true");
  require_fmt(false, "false");
}

TEST_CASE("formatting unsigned integer, base 10") {
  require_fmt(lim<std::uint8_t>::min(), "0");
  require_fmt(lim<std::uint8_t>::max(), "255");
  require_fmt(lim<std::uint16_t>::min(), "0");
  require_fmt(lim<std::uint16_t>::max(), "65535");
  require_fmt(lim<std::uint32_t>::min(), "0");
  require_fmt(lim<std::uint32_t>::max(), "4294967295");
  require_fmt(lim<std::uint64_t>::min(), "0");
  require_fmt(lim<std::uint64_t>::max(), "18446744073709551615");
}

TEST_CASE("formatting signed integer, base 10") {
  require_fmt(lim<std::int8_t>::min(), "-128");
  require_fmt(lim<std::int8_t>::max(), "127");
  require_fmt(lim<std::int16_t>::min(), "-32768");
  require_fmt(lim<std::int16_t>::max(), "32767");
  require_fmt(lim<std::int32_t>::min(), "-2147483648");
  require_fmt(lim<std::int32_t>::max(), "2147483647");
  require_fmt(lim<std::int64_t>::min(), "-9223372036854775808");
  require_fmt(lim<std::int64_t>::max(), "9223372036854775807");
}

TEST_CASE("formatting integer, base 2") {
  require_fmt(ff::bin(lim<std::uint8_t>::min()), "0");
  require_fmt(ff::bin(lim<std::uint8_t>::max()), "11111111");
  require_fmt(ff::bin(lim<std::int8_t>::min()), "-10000000");
  require_fmt(ff::bin(lim<std::int8_t>::max()), "1111111");
}

TEST_CASE("formatting integer, base 8") {
  require_fmt(ff::oct(lim<std::uint8_t>::min()), "0");
  require_fmt(ff::oct(lim<std::uint8_t>::max()), "377");
  require_fmt(ff::oct(lim<std::int8_t>::min()), "-200");
  require_fmt(ff::oct(lim<std::int8_t>::max()), "177");
}

TEST_CASE("formatting integer, base 16") {
  require_fmt(ff::hex(lim<std::uint8_t>::min()), "0");
  require_fmt(ff::hex(lim<std::uint8_t>::max()), "ff");
  require_fmt(ff::hex(lim<std::int8_t>::min()), "-80");
  require_fmt(ff::hex(lim<std::int8_t>::max()), "7f");
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
  require_fmt(person{"Alice", 42}, "Alice (age 42)");
  require_fmt(person{"Bob", 49}, "Bob (age 49)");
}
