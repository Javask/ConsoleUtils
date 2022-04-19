#include <catch2/catch.hpp>
#include "StringUtils.h"

TEST_CASE("Test to lower") {
  REQUIRE(toLowerCase("tEsTvAlUe") == "testvalue");
  REQUIRE(toLowerCase("TESTVALUE") == "testvalue");
  REQUIRE(toLowerCase("testvalue") == "testvalue");
  REQUIRE(toLowerCase("123456789") == "123456789");
}
TEST_CASE("Remove first and last char") {
  REQUIRE(removeFirstAndLastChar("test") == "es");
}
TEST_CASE("Remove first char") { REQUIRE(removeFirstChar("test") == "est"); }
TEST_CASE("Remove last char") { REQUIRE(removeLastChar("test") == "tes"); }
TEST_CASE("Split at spaces") {
  auto splits = splitAtSpaces("test");
  REQUIRE(splits.size() == 1);
  REQUIRE(splits[0] == "test");

  splits = splitAtSpaces("test1 test2 test3");
  REQUIRE(splits.size() == 3);
  REQUIRE(splits[0] == "test1");
  REQUIRE(splits[1] == "test2");
  REQUIRE(splits[2] == "test3");

  splits = splitAtSpaces(" test");
  REQUIRE(splits.size() == 1);
  REQUIRE(splits[0] == "test");

  splits = splitAtSpaces("test ");
  REQUIRE(splits.size() == 1);
  REQUIRE(splits[0] == "test");

  splits = splitAtSpaces("test1  test2");
  REQUIRE(splits.size() == 2);
  REQUIRE(splits[0] == "test1");
  REQUIRE(splits[1] == "test2");
}

TEST_CASE("Split strings with escape") {
  std::vector<std::string> splits;
  REQUIRE_NOTHROW(splits = splitAtSpacesWithEscape("test"));
  REQUIRE(splits.size() == 1);
  REQUIRE(splits[0] == "test");

  REQUIRE_NOTHROW(splits = splitAtSpacesWithEscape("test1 test2 test3"));
  REQUIRE(splits.size() == 3);
  REQUIRE(splits[0] == "test1");
  REQUIRE(splits[1] == "test2");
  REQUIRE(splits[2] == "test3");

  REQUIRE_NOTHROW(splits = splitAtSpacesWithEscape(" test"));
  REQUIRE(splits.size() == 1);
  REQUIRE(splits[0] == "test");

  REQUIRE_NOTHROW(splits = splitAtSpacesWithEscape("test "));
  REQUIRE(splits.size() == 1);
  REQUIRE(splits[0] == "test");

  REQUIRE_NOTHROW(splits = splitAtSpacesWithEscape("test1  test2"));
  REQUIRE(splits.size() == 2);
  REQUIRE(splits[0] == "test1");
  REQUIRE(splits[1] == "test2");

  REQUIRE_NOTHROW(splits =
                      splitAtSpacesWithEscape("test1 \"test2  test3\" test4"));
  REQUIRE(splits.size() == 3);
  REQUIRE(splits[0] == "test1");
  REQUIRE(splits[1] == "\"test2  test3\"");
  REQUIRE(splits[2] == "test4");

  REQUIRE_NOTHROW(splits =
                      splitAtSpacesWithEscape("\'test1 \"test1  test2\" \'"));
  REQUIRE(splits.size() == 1);
  REQUIRE(splits[0] == "\'test1 \"test1  test2\" \'");

  REQUIRE_THROWS(splitAtSpacesWithEscape("\'test1 \"test1\'  test2\""));
  REQUIRE_THROWS(splitAtSpacesWithEscape("\'test1 "));
  REQUIRE_THROWS(splitAtSpacesWithEscape("\"test1 "));
  REQUIRE_THROWS(splitAtSpacesWithEscape("test1\""));
  REQUIRE_THROWS(splitAtSpacesWithEscape("test1\'"));
}

TEST_CASE("Test is Integer") {
  REQUIRE(isInteger("0", true));
  REQUIRE(isInteger("0", false));
  REQUIRE(isInteger("1", true));
  REQUIRE(isInteger("1", false));
  REQUIRE(isInteger("123"));
  REQUIRE(!isInteger("123a"));
  REQUIRE(isInteger("-1", true));
  REQUIRE(!isInteger("-1", false));
  REQUIRE(!isInteger("0.0", true));
  REQUIRE(isInteger("+1", false));
  REQUIRE(!isInteger(""));
  REQUIRE(!isInteger("a"));
}
TEST_CASE("Test is Real") {
  REQUIRE(isReal("0"));
  REQUIRE(isReal("0.0"));
  REQUIRE(isReal("1.0"));
  REQUIRE(isReal("10.0"));
  REQUIRE(isReal("-10.0"));
  REQUIRE(isReal("+10.0"));
  REQUIRE(isReal("0.5"));
  REQUIRE(isReal(".5"));
  REQUIRE(isReal("-.5"));
  REQUIRE(!isReal(""));
  REQUIRE(!isReal("a"));
  REQUIRE(!isReal("0.a"));
  REQUIRE(!isReal("a.0"));
  REQUIRE(!isReal("0a0"));

  REQUIRE(!isReal("-1.0", false));
  REQUIRE(isReal("1.0", true));
}