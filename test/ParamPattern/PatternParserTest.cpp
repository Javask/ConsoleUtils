#include <catch2/catch.hpp>
#include "ParamPattern/PatternParser.h"
using namespace ConsoleUtils;
TEST_CASE("Pattern Parser works") {
  REQUIRE_THROWS(PatternParser::parse(""));
  std::shared_ptr<PatternToken> tokenPtr = nullptr;

  REQUIRE_NOTHROW(tokenPtr = PatternParser::parse("test"));
  REQUIRE(tokenPtr->getType() == PatternTokenType::VALUE);
  {
    auto valPtr = std::dynamic_pointer_cast<ValueToken>(tokenPtr);
    REQUIRE(valPtr);
    REQUIRE(valPtr->getName() == "test");
  }

  REQUIRE_NOTHROW(tokenPtr = PatternParser::parse("test test2"));
  REQUIRE(tokenPtr->getType() == PatternTokenType::ORDER);
  {
    auto valPtr = std::dynamic_pointer_cast<OrderToken>(tokenPtr);
    REQUIRE(valPtr);
    REQUIRE(valPtr->getChildren().size() >= 1);
    REQUIRE(valPtr->getChildren()[0]->getType() == PatternTokenType::VALUE);
    REQUIRE(std::static_pointer_cast<ValueToken>(valPtr->getChildren()[0])
                ->getName() == "test");
    REQUIRE(valPtr->getChildren().size() == 2);
  }

  REQUIRE_NOTHROW(tokenPtr = PatternParser::parse("[test test]"));
  REQUIRE(tokenPtr->getType() == PatternTokenType::OPTIONAL);
  {
    auto valPtr = std::dynamic_pointer_cast<OptionalToken>(tokenPtr);
    REQUIRE(valPtr);
    REQUIRE(valPtr->getChildren().size() == 1);
  }

  REQUIRE_NOTHROW(tokenPtr = PatternParser::parse("--test"));
  REQUIRE(tokenPtr->getType() == PatternTokenType::OPTION);
  {
    auto valPtr = std::dynamic_pointer_cast<OptionToken>(tokenPtr);
    REQUIRE(valPtr);
    REQUIRE(valPtr->getName() == "--test");
    REQUIRE(valPtr->getArgType() == ArgType::None);
  }

  REQUIRE_NOTHROW(tokenPtr = PatternParser::parse("--test <string>"));
  REQUIRE(tokenPtr->getType() == PatternTokenType::OPTION);
  {
    auto valPtr = std::dynamic_pointer_cast<OptionToken>(tokenPtr);
    REQUIRE(valPtr);
    REQUIRE(valPtr->getName() == "--test");
    REQUIRE(valPtr->getArgType() == ArgType::String);
  }

  REQUIRE_NOTHROW(tokenPtr = PatternParser::parse("test | [test2]"));
  REQUIRE(tokenPtr->getType() == PatternTokenType::OR);
  {
    auto valPtr = std::dynamic_pointer_cast<OrToken>(tokenPtr);
    REQUIRE(valPtr);
    REQUIRE(valPtr->getChildren().size() == 2);
  }

  REQUIRE_NOTHROW(tokenPtr = PatternParser::parse("test ..."));
  REQUIRE(tokenPtr->getType() == PatternTokenType::MULTIPLE);
  {
    auto valPtr = std::dynamic_pointer_cast<MultipleToken>(tokenPtr);
    REQUIRE(valPtr);
    REQUIRE(valPtr->getName() == "test");
  }
}