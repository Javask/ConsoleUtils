#include "ConsoleUtils/ParamParser.h"
#include <catch2/catch.hpp>
#include <cstring>

TEST_CASE("test param parser") {
  std::string pattern1 = "name1";
  std::vector<ParamDescription> descriptions1 =
      std::vector<ParamDescription>(1);
  descriptions1[0].name = "name1";
  descriptions1[0].description = "test";
  auto parser = ParamParser(pattern1, descriptions1);
  parser.setOutput(nullptr);
  std::string testname0 = "consolename";
  std::string testname1 = "a";
  char** input = new char*[2];
  char* val0 = strcpy(new char[testname0.size() + 1], testname0.c_str());
  char* val1 = strcpy(new char[testname1.size() + 1], testname1.c_str());
  input[0] = val0;
  input[1] = val1;
  REQUIRE_THROWS(parser.parse(0, input));
  REQUIRE_THROWS(parser.parse(1, input));
  ParamResult paramRes = ParamResult::createResultShouldExit();
  ParameterValues res;
  REQUIRE_NOTHROW(paramRes = parser.parse(2, input));
  REQUIRE(!paramRes.shouldExit());
  res = paramRes.getValues();
  REQUIRE(res.size() == 1);
  REQUIRE(res.contains("name1"));
  REQUIRE(res["name1"].size() == 1);
  REQUIRE(res["name1"][0] == "a");

  delete[] input;
  delete[] val0;
  delete[] val1;
}