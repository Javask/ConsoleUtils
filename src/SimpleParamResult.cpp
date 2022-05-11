#include "ConsoleUtils/SimpleParamResult.h"
namespace ConsoleUtils {
SimpleParamResult SimpleParamResult::createResultShouldExit() {
  return SimpleParamResult("", {}, true);
}

SimpleParamResult SimpleParamResult::create(std::string patternID,
                                            ParameterValues values) {
  return SimpleParamResult(patternID, values, false);
}

SimpleParamResult::SimpleParamResult(std::string patternID,
                                     ParameterValues values, bool exit)
    : patternID(patternID), values(values), exit(exit) {}

std::string SimpleParamResult::getPatternID() const { return patternID; }

ParameterValues SimpleParamResult::getValues() const { return values; }

bool SimpleParamResult::shouldExit() const { return exit; }
}  // namespace ConsoleUtils