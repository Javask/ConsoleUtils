#include "ConsoleUtils/ConstExpr/StringUtils.h"
#include <catch2/catch.hpp>
using namespace ConsoleUtils::ConstExpr::String;

constexpr char str[14] = "  test test  ";

TEST_CASE("Test constexpr split string") {
  REQUIRE(getSeqmentCount<14>(str) == 2);
  auto test = std::array<std::basic_string<char>, 2>({"test", "test"});
  auto test2 = splitAtSpaces<14, str>();
  REQUIRE(test2[0] == test[0]);
  REQUIRE(test2[1] == test[1]);
}