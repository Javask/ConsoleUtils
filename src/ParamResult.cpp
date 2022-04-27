#include "ConsoleUtils/ParamResult.h"

namespace ConsoleUtils {
ParamResult::ParamResult(ParameterValues values, bool exit)
    : values(values), exit(exit) {}

ParamResult ParamResult::createResultShouldExit() {
  return ParamResult({}, true);
}

ParameterValues ParamResult::getValues() { return values; }

bool ParamResult::shouldExit() { return exit; }

ParamResult ParamResult::create(ParameterValues values) {
  return ParamResult(values, false);
}
}  // namespace ConsoleUtils
