#pragma once
#include <string>
#include "ArgType.h"

namespace ConsoleUtils {

struct PatternToken {
  std::string name = "";
  bool isOptional = false;
  bool option = false;
  bool canBeMultiple = false;
  ArgType argtype = ArgType::None;
};

}  // namespace ConsoleUtils
