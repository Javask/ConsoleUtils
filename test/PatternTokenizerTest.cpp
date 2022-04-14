#include <catch2/catch.hpp>
#include "PatternTokenizer.h"

TEST_CASE("Test name is option") {
  std::string optionname = "[--test]";
  std::string optionname2 = "[-test]";
  std::string optionname3 = "[-]";
  std::string paramname = "[test]";
  std::string argtype = "<--test>";
  std::vector<PatternToken> tokens;
  REQUIRE_NOTHROW(tokens = PatternTokenizer::tokenize(optionname));
  REQUIRE(tokens.size() == 1);
  REQUIRE(tokens[0].name == "--test");
  REQUIRE(tokens[0].option == true);
  REQUIRE(tokens[0].isOptional == true);
  REQUIRE(tokens[0].canBeMultiple == false);
  REQUIRE(tokens[0].argtype == ArgType::None);

  REQUIRE_NOTHROW(tokens = PatternTokenizer::tokenize(optionname2));
  REQUIRE(tokens.size() == 1);
  REQUIRE(tokens[0].name == "-test");
  REQUIRE(tokens[0].option == true);
  REQUIRE(tokens[0].isOptional == true);
  REQUIRE(tokens[0].canBeMultiple == false);
  REQUIRE(tokens[0].argtype == ArgType::None);

  REQUIRE_THROWS(PatternTokenizer::tokenize(optionname3));
  REQUIRE_NOTHROW(tokens = PatternTokenizer::tokenize(paramname));
  REQUIRE(tokens.size() == 1);
  REQUIRE(tokens[0].name == "test");
  REQUIRE(tokens[0].option == false);
  REQUIRE(tokens[0].isOptional == true);
  REQUIRE(tokens[0].canBeMultiple == false);
  REQUIRE(tokens[0].argtype == ArgType::None);

  REQUIRE_THROWS(PatternTokenizer::tokenize(argtype));
}

TEST_CASE("Test split is optional") {
  std::string param = "test";
  std::string option = "--test";
  std::string option2 = "-test";
  std::string optparam = "[test]";
  std::string optoption = "[--test]";
  std::string optoption2 = "[-test]";
  std::vector<PatternToken> tokens;
  REQUIRE_NOTHROW(tokens = PatternTokenizer::tokenize(param));
  REQUIRE(tokens.size() == 1);
  REQUIRE(tokens[0].name == "test");
  REQUIRE(tokens[0].option == false);
  REQUIRE(tokens[0].isOptional == false);
  REQUIRE(tokens[0].canBeMultiple == false);
  REQUIRE(tokens[0].argtype == ArgType::None);

  REQUIRE_THROWS(PatternTokenizer::tokenize(option));
  REQUIRE_THROWS(PatternTokenizer::tokenize(option2));
  REQUIRE_NOTHROW(tokens = PatternTokenizer::tokenize(optparam));
  REQUIRE(tokens.size() == 1);
  REQUIRE(tokens[0].name == "test");
  REQUIRE(tokens[0].option == false);
  REQUIRE(tokens[0].isOptional == true);
  REQUIRE(tokens[0].canBeMultiple == false);
  REQUIRE(tokens[0].argtype == ArgType::None);

  REQUIRE_NOTHROW(tokens = PatternTokenizer::tokenize(optoption));
  REQUIRE(tokens.size() == 1);
  REQUIRE(tokens[0].name == "--test");
  REQUIRE(tokens[0].option == true);
  REQUIRE(tokens[0].isOptional == true);
  REQUIRE(tokens[0].canBeMultiple == false);
  REQUIRE(tokens[0].argtype == ArgType::None);

  REQUIRE_NOTHROW(tokens = PatternTokenizer::tokenize(optoption2));
  REQUIRE(tokens.size() == 1);
  REQUIRE(tokens[0].name == "-test");
  REQUIRE(tokens[0].option == true);
  REQUIRE(tokens[0].isOptional == true);
  REQUIRE(tokens[0].canBeMultiple == false);
  REQUIRE(tokens[0].argtype == ArgType::None);
}

TEST_CASE("Test arg types") {
  std::string str1 = "[--name <string>]";
  std::string str2 = "[--name <bool>]";
  std::string str3 = "[--name <int>]";
  std::string str4 = "[--name <real>]";
  std::string str5 = "[--name <+int>]";
  std::string str6 = "[--name <+real>]";
  std::string str7 = "[--name <test>]";
  std::string str8 = "[--name <+string>]";
  std::vector<PatternToken> tokens;
  REQUIRE_NOTHROW(tokens = PatternTokenizer::tokenize(str1));
  REQUIRE(tokens.size() == 1);
  REQUIRE(tokens[0].name == "--name");
  REQUIRE(tokens[0].option == true);
  REQUIRE(tokens[0].isOptional == true);
  REQUIRE(tokens[0].canBeMultiple == false);
  REQUIRE(tokens[0].argtype == ArgType::String);

  REQUIRE_NOTHROW(tokens = PatternTokenizer::tokenize(str2));
  REQUIRE(tokens.size() == 1);
  REQUIRE(tokens[0].name == "--name");
  REQUIRE(tokens[0].option == true);
  REQUIRE(tokens[0].isOptional == true);
  REQUIRE(tokens[0].canBeMultiple == false);
  REQUIRE(tokens[0].argtype == ArgType::Boolean);

  REQUIRE_NOTHROW(tokens = PatternTokenizer::tokenize(str3));
  REQUIRE(tokens.size() == 1);
  REQUIRE(tokens[0].name == "--name");
  REQUIRE(tokens[0].option == true);
  REQUIRE(tokens[0].isOptional == true);
  REQUIRE(tokens[0].canBeMultiple == false);
  REQUIRE(tokens[0].argtype == ArgType::Integer);

  REQUIRE_NOTHROW(tokens = PatternTokenizer::tokenize(str4));
  REQUIRE(tokens.size() == 1);
  REQUIRE(tokens[0].name == "--name");
  REQUIRE(tokens[0].option == true);
  REQUIRE(tokens[0].isOptional == true);
  REQUIRE(tokens[0].canBeMultiple == false);
  REQUIRE(tokens[0].argtype == ArgType::Real);

  REQUIRE_NOTHROW(tokens = PatternTokenizer::tokenize(str5));
  REQUIRE(tokens.size() == 1);
  REQUIRE(tokens[0].name == "--name");
  REQUIRE(tokens[0].option == true);
  REQUIRE(tokens[0].isOptional == true);
  REQUIRE(tokens[0].canBeMultiple == false);
  REQUIRE(tokens[0].argtype == ArgType::PositiveInteger);

  REQUIRE_NOTHROW(tokens = PatternTokenizer::tokenize(str6));
  REQUIRE(tokens.size() == 1);
  REQUIRE(tokens[0].name == "--name");
  REQUIRE(tokens[0].option == true);
  REQUIRE(tokens[0].isOptional == true);
  REQUIRE(tokens[0].canBeMultiple == false);
  REQUIRE(tokens[0].argtype == ArgType::PositiveReal);

  REQUIRE_THROWS(PatternTokenizer::tokenize(str7));
  REQUIRE_THROWS(PatternTokenizer::tokenize(str8));
}

TEST_CASE("Test param names") {
  std::vector<PatternToken> tokens;
  REQUIRE_NOTHROW(tokens = PatternTokenizer::tokenize("test"));
  REQUIRE(tokens.size() == 1);
  REQUIRE(tokens[0].name == "test");
  REQUIRE(tokens[0].option == false);
  REQUIRE(tokens[0].isOptional == false);
  REQUIRE(tokens[0].canBeMultiple == false);
  REQUIRE(tokens[0].argtype == ArgType::None);

  REQUIRE_NOTHROW(tokens = PatternTokenizer::tokenize("test2"));
  REQUIRE(tokens.size() == 1);
  REQUIRE(tokens[0].name == "test2");
  REQUIRE(tokens[0].option == false);
  REQUIRE(tokens[0].isOptional == false);
  REQUIRE(tokens[0].canBeMultiple == false);
  REQUIRE(tokens[0].argtype == ArgType::None);

  REQUIRE_NOTHROW(tokens = PatternTokenizer::tokenize("test-"));
  REQUIRE(tokens.size() == 1);
  REQUIRE(tokens[0].name == "test-");
  REQUIRE(tokens[0].option == false);
  REQUIRE(tokens[0].isOptional == false);
  REQUIRE(tokens[0].canBeMultiple == false);
  REQUIRE(tokens[0].argtype == ArgType::None);

  REQUIRE_NOTHROW(tokens = PatternTokenizer::tokenize("test_"));
  REQUIRE(tokens.size() == 1);
  REQUIRE(tokens[0].name == "test_");
  REQUIRE(tokens[0].option == false);
  REQUIRE(tokens[0].isOptional == false);
  REQUIRE(tokens[0].canBeMultiple == false);
  REQUIRE(tokens[0].argtype == ArgType::None);

  REQUIRE_THROWS(PatternTokenizer::tokenize("-test"));
  REQUIRE_THROWS(PatternTokenizer::tokenize("2test"));
}

TEST_CASE("Test option names") {
  std::vector<PatternToken> tokens;
  REQUIRE_NOTHROW(tokens = PatternTokenizer::tokenize("[test]"));
  REQUIRE(tokens.size() == 1);
  REQUIRE(tokens[0].name == "test");
  REQUIRE(tokens[0].option == false);
  REQUIRE(tokens[0].isOptional == true);
  REQUIRE(tokens[0].canBeMultiple == false);
  REQUIRE(tokens[0].argtype == ArgType::None);

  REQUIRE_NOTHROW(tokens = PatternTokenizer::tokenize("[test2]"));
  REQUIRE(tokens.size() == 1);
  REQUIRE(tokens[0].name == "test2");
  REQUIRE(tokens[0].option == false);
  REQUIRE(tokens[0].isOptional == true);
  REQUIRE(tokens[0].canBeMultiple == false);
  REQUIRE(tokens[0].argtype == ArgType::None);

  REQUIRE_NOTHROW(tokens = PatternTokenizer::tokenize("[test-]"));
  REQUIRE(tokens.size() == 1);
  REQUIRE(tokens[0].name == "test-");
  REQUIRE(tokens[0].option == false);
  REQUIRE(tokens[0].isOptional == true);
  REQUIRE(tokens[0].canBeMultiple == false);
  REQUIRE(tokens[0].argtype == ArgType::None);

  REQUIRE_NOTHROW(tokens = PatternTokenizer::tokenize("[test_]"));
  REQUIRE(tokens.size() == 1);
  REQUIRE(tokens[0].name == "test_");
  REQUIRE(tokens[0].option == false);
  REQUIRE(tokens[0].isOptional == true);
  REQUIRE(tokens[0].canBeMultiple == false);
  REQUIRE(tokens[0].argtype == ArgType::None);

  REQUIRE_NOTHROW(tokens = PatternTokenizer::tokenize("[-test]"));
  REQUIRE(tokens.size() == 1);
  REQUIRE(tokens[0].name == "-test");
  REQUIRE(tokens[0].option == true);
  REQUIRE(tokens[0].isOptional == true);
  REQUIRE(tokens[0].canBeMultiple == false);
  REQUIRE(tokens[0].argtype == ArgType::None);

  REQUIRE_NOTHROW(tokens = PatternTokenizer::tokenize("[-2]"));
  REQUIRE(tokens.size() == 1);
  REQUIRE(tokens[0].name == "-2");
  REQUIRE(tokens[0].option == true);
  REQUIRE(tokens[0].isOptional == true);
  REQUIRE(tokens[0].canBeMultiple == false);
  REQUIRE(tokens[0].argtype == ArgType::None);

  REQUIRE_NOTHROW(tokens = PatternTokenizer::tokenize("[--2]"));
  REQUIRE(tokens.size() == 1);
  REQUIRE(tokens[0].name == "--2");
  REQUIRE(tokens[0].option == true);
  REQUIRE(tokens[0].isOptional == true);
  REQUIRE(tokens[0].canBeMultiple == false);
  REQUIRE(tokens[0].argtype == ArgType::None);

  REQUIRE_NOTHROW(tokens = PatternTokenizer::tokenize("[--test]"));
  REQUIRE(tokens.size() == 1);
  REQUIRE(tokens[0].name == "--test");
  REQUIRE(tokens[0].option == true);
  REQUIRE(tokens[0].isOptional == true);
  REQUIRE(tokens[0].canBeMultiple == false);
  REQUIRE(tokens[0].argtype == ArgType::None);

  REQUIRE_NOTHROW(tokens = PatternTokenizer::tokenize("[--2test]"));
  REQUIRE(tokens.size() == 1);
  REQUIRE(tokens[0].name == "--2test");
  REQUIRE(tokens[0].option == true);
  REQUIRE(tokens[0].isOptional == true);
  REQUIRE(tokens[0].canBeMultiple == false);
  REQUIRE(tokens[0].argtype == ArgType::None);

  REQUIRE_THROWS(PatternTokenizer::tokenize("[----]"));
  REQUIRE_THROWS(PatternTokenizer::tokenize("[-]"));
  REQUIRE_THROWS(PatternTokenizer::tokenize("[2test]"));
}

TEST_CASE("Tokenize pattern") {
  std::string pattern1 = "name [name2] [--name3] [--name4 <string>] name5 ...";
  std::string pattern2 = "name ... [--name2]";
  std::string pattern3 = "name [name2 ...]";
  std::vector<PatternToken> tokens;
  REQUIRE_NOTHROW(tokens = PatternTokenizer::tokenize(pattern1));
  REQUIRE(tokens.size() == 5);
  REQUIRE(tokens[0].name == "name");
  REQUIRE(tokens[0].canBeMultiple == false);
  REQUIRE(tokens[0].isOptional == false);
  REQUIRE(tokens[0].argtype == ArgType::None);
  REQUIRE(tokens[0].option == false);

  REQUIRE(tokens[1].name == "name2");
  REQUIRE(tokens[1].canBeMultiple == false);
  REQUIRE(tokens[1].isOptional == true);
  REQUIRE(tokens[1].argtype == ArgType::None);
  REQUIRE(tokens[1].option == false);

  REQUIRE(tokens[2].name == "--name3");
  REQUIRE(tokens[2].canBeMultiple == false);
  REQUIRE(tokens[2].isOptional == true);
  REQUIRE(tokens[2].argtype == ArgType::None);
  REQUIRE(tokens[2].option == true);

  REQUIRE(tokens[3].name == "--name4");
  REQUIRE(tokens[3].canBeMultiple == false);
  REQUIRE(tokens[3].isOptional == true);
  REQUIRE(tokens[3].argtype == ArgType::String);
  REQUIRE(tokens[3].option == true);

  REQUIRE(tokens[4].name == "name5");
  REQUIRE(tokens[4].canBeMultiple == true);
  REQUIRE(tokens[4].isOptional == false);
  REQUIRE(tokens[4].argtype == ArgType::None);
  REQUIRE(tokens[4].option == false);

  REQUIRE_THROWS(PatternTokenizer::tokenize(pattern2));
  REQUIRE_NOTHROW(tokens = PatternTokenizer::tokenize(pattern3));
  REQUIRE(tokens.size() == 2);
  REQUIRE(tokens[0].name == "name");
  REQUIRE(tokens[0].canBeMultiple == false);
  REQUIRE(tokens[0].isOptional == false);
  REQUIRE(tokens[0].argtype == ArgType::None);
  REQUIRE(tokens[0].option == false);

  REQUIRE(tokens[1].name == "name2");
  REQUIRE(tokens[1].canBeMultiple == true);
  REQUIRE(tokens[1].isOptional == true);
  REQUIRE(tokens[1].argtype == ArgType::None);
  REQUIRE(tokens[1].option == false);
}