#pragma once
#include "ParameterValues.h"

namespace ConsoleUtils {

class SimpleParamResult {
 public:
  static SimpleParamResult create(std::string patternID,
                                  ParameterValues values);
  static SimpleParamResult createResultShouldExit();
  ~SimpleParamResult() = default;

  std::string getPatternID() const;
  ParameterValues getValues() const;
  bool shouldExit() const;

 private:
  SimpleParamResult(std::string patternID, ParameterValues values, bool exit);

  const std::string patternID;
  const ParameterValues values;
  const bool exit;
};

}  // namespace ConsoleUtils