#pragma once
#include "PatternToken.h"
namespace ConsoleUtils {
class PatternValidator {
 private:
  PatternValidator() = default;
  ~PatternValidator() = default;

 public:
  static bool validate(const PatternToken& token);

 private:
};
}  // namespace ConsoleUtils