#pragma once
#include <vector>
#include <string>
#include "ArgType.h"
#include "SimpleParamResult.h"
#include <map>
#include <ostream>

namespace ConsoleUtils {

class SimpleParamParser {
 public:
  struct Option {
    std::string name;
    ArgType type = ArgType::None;
  };

  struct Parameter {
    std::string name;
    bool optional;
    bool multiple;
  };

  struct Pattern {
    std::string patternID;
    std::vector<Option> options;
    std::vector<Parameter> values;
    std::map<std::string, std::string> descriptions;
  };

  SimpleParamParser(std::ostream* cout);
  ~SimpleParamParser() = default;

  void addPattern(const Pattern& pattern);
  SimpleParamResult parse(int argc, char** argv);
  SimpleParamResult parse(const std::string& arguments);

 private:
  void printHelp();
  std::vector<Pattern> patterns;
  std::ostream* console;
};

}  // namespace ConsoleUtils