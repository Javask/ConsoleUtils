#include "TokenUtils.h"
#include <regex>

const std::regex paramNameRegex = std::regex("[[:alpha:]][\\w_-]*");
const std::regex optionNameRegex = std::regex("-+\\w[\\w_-]*");

bool isValidOptionName(const std::string& val) {
  return std::regex_match(val, optionNameRegex);
}

bool isValidParamName(const std::string& val) {
  return std::regex_match(val, paramNameRegex);
}

bool isArgType(const std::string& val) {
  return val.size() >= 3 && val[0] == '<' && val[val.size() - 1] == '>';
}
