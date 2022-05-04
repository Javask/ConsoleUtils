#pragma once
#include "PatternToken.h"
namespace ConsoleUtils {
class PatternParser {
 private:
  PatternParser() = default;
  ~PatternParser() = default;

 public:
  static std::shared_ptr<PatternToken> parse(const std::string& pattern);

 private:
  static std::vector<std::string> splitOptional(const std::string& pattern);
  static std::vector<std::string> splitOrder(const std::string& pattern);
  static std::vector<std::string> splitOr(const std::string& pattern);

  static std::vector<std::string> splitAtIndices(
      const std::string& pattern, const std::vector<size_t> splitPoints);

  static std::string getValueName(const std::string& pattern);
  static std::string getMultipleName(const std::string& pattern);
  static std::pair<std::string, ArgType> getOptionNameAndArgType(
      const std::string& pattern);

  static bool isOptional(const std::string& pattern);
  static bool isOption(const std::string& pattern);
  static bool isOrder(const std::string& pattern);
  static bool isOr(const std::string& pattern);
  static bool isValue(const std::string& pattern);
  static bool isMultiple(const std::string& pattern);
};

}  // namespace ConsoleUtils