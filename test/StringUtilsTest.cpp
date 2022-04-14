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