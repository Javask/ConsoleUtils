#include <catch2/catch_test_macros.hpp>
#include "ConsoleUtils/SimpleParamParser.h"
using namespace ConsoleUtils;
// TODO test

TEST_CASE("Simple param parser options works") {
  SimpleParamParser::Option testOptNone;
  testOptNone.name = "--testNone";
  testOptNone.type = ArgType::None;

  SimpleParamParser::Option testOptReal;
  testOptReal.name = "--testReal";
  testOptReal.type = ArgType::Real;

  SimpleParamParser::Pattern pattern;
  pattern.patternID = "test1";
  pattern.descriptions["--testNone"] = "test none desc";
  pattern.descriptions["--testReal"] = "test real desc";
  pattern.options.push_back(testOptNone);
  pattern.options.push_back(testOptReal);

  SimpleParamParser parser(nullptr);
  parser.addPattern(pattern);
  SECTION("help") {
    auto res = parser.parse("--help");
    REQUIRE(res.shouldExit() == true);
  }
  SECTION("invalid opt") {
    auto res = parser.parse("--test");
    REQUIRE(res.shouldExit() == true);
  }
  SECTION("none opt") {
    auto res = parser.parse("--testNone");
    REQUIRE(res.shouldExit() == false);
    REQUIRE(res.getValues().contains("--testNone"));
    REQUIRE(res.getValues()["--testNone"].size() == 0);
  }
  SECTION("real opt") {
    auto res = parser.parse("--testReal");
    REQUIRE(res.shouldExit() == true);
    auto res2 = parser.parse("--testReal 0.2");
    REQUIRE(res2.shouldExit() == false);
    REQUIRE(res2.getValues().contains("--testReal"));
    REQUIRE(res2.getValues()["--testReal"].size() == 1);
    REQUIRE(res2.getValues()["--testReal"][0] == "0.2");
  }
  SECTION("empty") {
    auto res = parser.parse("");
    REQUIRE(res.shouldExit() == false);
  }
}

TEST_CASE("Simple param parser values works") {
  SimpleParamParser::Parameter testSimple;
  testSimple.name = "testSimple";
  testSimple.multiple = false;
  testSimple.optional = false;

  SimpleParamParser::Parameter testOpt;
  testOpt.name = "testOpt";
  testOpt.multiple = false;
  testOpt.optional = true;

  SimpleParamParser::Parameter testMult;
  testMult.name = "testMult";
  testMult.multiple = true;
  testMult.optional = false;

  SimpleParamParser::Pattern pattern;
  pattern.patternID = "test1";
  pattern.descriptions["testSimple"] = "testSimple desc";
  pattern.descriptions["testOpt"] = "testOpt desc";
  pattern.descriptions["testMult"] = "testMult desc";
  pattern.values.push_back(testSimple);
  pattern.values.push_back(testOpt);
  pattern.values.push_back(testMult);

  SimpleParamParser parser(nullptr);
  parser.addPattern(pattern);
  {
    auto res = parser.parse("--test test test2");
    REQUIRE(res.shouldExit() == true);
  }

  {
    auto res = parser.parse("test test2 test3 test4");
    REQUIRE(res.shouldExit() == false);
    auto vals = res.getValues();
    REQUIRE(vals.contains("testSimple"));
    REQUIRE(vals.contains("testMult"));
    REQUIRE(vals["testMult"].size() == 2);
    REQUIRE(vals.contains("testOpt"));
  }
}