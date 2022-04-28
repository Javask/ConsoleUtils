#pragma once
#include <string>

namespace ConsoleUtils {
enum class ArgType {
  String,
  Real,
  PositiveReal,
  Integer,
  PositiveInteger,
  Boolean,
  None
};

ArgType toArgType(const std::string& val);
}  // namespace ConsoleUtils