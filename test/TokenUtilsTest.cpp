#include "TokenUtils.h"
#include <catch2/catch.hpp>

TEST_CASE("Test isOptional") {
  REQUIRE(!isOptional("test"));
  REQUIRE(!isOptional("[]"));
  REQUIRE(isOptional("[test]"));
  REQUIRE(isOptional("[--test]"));
}

TEST_CASE("Test option") {
  REQUIRE(!isOption("test"));
  REQUIRE(isOption("-test"));
  REQUIRE(isOption("--test"));
  REQUIRE(isOption("---test"));
  REQUIRE(!isOption("---"));
  REQUIRE(!isOption("-"));
}

TEST_CASE("Test isArgType") {
  REQUIRE(!isArgType("test"));
  REQUIRE(!isArgType("<test"));
  REQUIRE(!isArgType("test>"));
  REQUIRE(isArgType("<test>"));
  REQUIRE(!isArgType("<>"));
}

TEST_CASE("Test valid paramnames") {
  REQUIRE(isValidParamName("a"));
  REQUIRE(isValidParamName("test"));
  REQUIRE(isValidParamName("a2"));
  REQUIRE(isValidParamName("a2-_"));
  REQUIRE(!isValidParamName("-a"));
  REQUIRE(!isValidParamName(""));
  REQUIRE(!isValidParamName("-2"));
  REQUIRE(!isValidParamName("2"));
}
TEST_CASE("Test valid optionnames") {
  REQUIRE(!isValidOptionName("a"));
  REQUIRE(!isValidOptionName("test"));
  REQUIRE(!isValidOptionName("a2"));
  REQUIRE(!isValidOptionName("a2-_"));
  REQUIRE(isValidOptionName("-a"));
  REQUIRE(!isValidOptionName(""));
  REQUIRE(isValidOptionName("-2"));
  REQUIRE(!isValidOptionName("2"));
  REQUIRE(isValidOptionName("-a2-_"));
  REQUIRE(!isValidOptionName("--"));
  REQUIRE(!isValidOptionName("-"));
}