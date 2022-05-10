#pragma once
#include "ConsoleUtils/ParamResult.h"
#include "PatternToken.h"

namespace ConsoleUtils {
class InputParser {
 private:
  InputParser() = default;
  ~InputParser() = default;

 public:
  static ParamResult parse(std::shared_ptr<PatternToken> pattern,
                           const std::string& input);
};

}  // namespace ConsoleUtils