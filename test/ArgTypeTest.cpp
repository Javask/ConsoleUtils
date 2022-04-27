#include <catch2/catch.hpp>
#include "ArgType.h"
using namespace ConsoleUtils;
TEST_CASE("String to arg_type conversion") {
  REQUIRE(toArgType("string") == ArgType::String);
  REQUIRE(toArgType("int") == ArgType::Integer);
  REQUIRE(toArgType("+int") == ArgType::PositiveInteger);
  REQUIRE(toArgType("real") == ArgType::Real);
  REQUIRE(toArgType("+real") == ArgType::PositiveReal);
  REQUIRE(toArgType("bool") == ArgType::Boolean);
  REQUIRE(toArgType("blaaaaa") == ArgType::None);
}