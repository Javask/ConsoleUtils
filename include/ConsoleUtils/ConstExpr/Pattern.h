#pragma once
#include <vector>
#include <array>
#include <string>
#include <cassert>
#include "../PatternToken.h"

namespace ConsoleUtils::ConstExpr {

enum class ArgType {
  String,
  Real,
  PositiveReal,
  Integer,
  PositiveInteger,
  Boolean,
  None
};

class Pattern {
 public:
  ~Pattern() = default;

 private:
  const std::vector<PatternToken> tokens;
};

}  // namespace ConsoleUtils::ConstExpr
