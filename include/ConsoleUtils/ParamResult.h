#pragma once
#include "ParameterValues.h"

namespace ConsoleUtils {

class ParamResult {
 public:
  static ParamResult create(ParameterValues values);
  static ParamResult createResultShouldExit();
  ~ParamResult() = default;
  ParameterValues getValues();
  bool shouldExit();

 private:
  ParamResult(ParameterValues values, bool exit);
  ParameterValues values;
  bool exit;
};

}  // namespace ConsoleUtils