#include "PatternParser.h"
#include <Utilities/StringUtils.h>
#include "TokenUtils.h"

namespace ConsoleUtils {
std::shared_ptr<PatternToken> PatternParser::parse(const std::string& pattern) {
  std::string trimmed = pattern;
  trimmed.erase(trimmed.find_last_not_of(' ') + 1);
  trimmed.erase(0, trimmed.find_first_not_of(' '));

  if (isOption(trimmed)) {
    auto pair = getOptionNameAndArgType(trimmed);
    return std::make_shared<OptionToken>(pair.first, pair.second);

  } else if (isValue(trimmed)) {
    auto name = getValueName(trimmed);
    return std::make_shared<ValueToken>(name);

  } else if (isMultiple(trimmed)) {
    auto name = getMultipleName(trimmed);
    return std::make_shared<MultipleToken>(name);

  } else if (isOrder(trimmed)) {
    auto splits = splitOrder(trimmed);
    std::vector<std::shared_ptr<PatternToken>> children;
    children.reserve(splits.size());
    for (auto& split : splits) {
      children.push_back(parse(split));
    }
    return std::make_shared<OrderToken>(children);

  } else if (isOr(trimmed)) {
    auto splits = splitOr(trimmed);
    std::vector<std::shared_ptr<PatternToken>> children;
    children.reserve(splits.size());
    for (auto& split : splits) {
      children.push_back(parse(split));
    }
    return std::make_shared<OrToken>(children);

  } else if (isOptional(trimmed)) {
    auto splits = splitOptional(trimmed);
    std::vector<std::shared_ptr<PatternToken>> children;
    children.reserve(splits.size());
    for (auto& split : splits) {
      children.push_back(parse(split));
    }
    return std::make_shared<OptionalToken>(children);
  }
  throw std::invalid_argument("Could not parse pattern \"" + pattern + "\"");
}

std::vector<std::string> PatternParser::splitOptional(
    const std::string& pattern) {
  return {Utilities::removeFirstAndLastChar(pattern)};
}

std::vector<std::string> PatternParser::splitOrder(const std::string& pattern) {
  bool inStringDouble = false;
  bool inStringSingle = false;
  size_t optionalLevel = 0;
  std::vector<size_t> splitPoints;
  for (size_t i = 0; i < pattern.size(); i++) {
    if (pattern[i] == '[' && !inStringDouble && !inStringSingle) {
      optionalLevel++;
    } else if (pattern[i] == ']' && !inStringDouble && !inStringSingle) {
      if (optionalLevel == 0) {
        throw std::invalid_argument("Unbalanced number or [] in pattern!");
      }
      optionalLevel--;
    } else if (pattern[i] == '\"' && !inStringSingle) {
      inStringDouble = !inStringDouble;
    } else if (pattern[i] == '\'' && !inStringDouble) {
      inStringSingle = !inStringSingle;
    } else if (pattern[i] == ' ' && !inStringDouble && !inStringSingle &&
               optionalLevel == 0) {
      splitPoints.push_back(i);
    }
  }
  auto splits = splitAtIndices(pattern, splitPoints);
  if (splits.size() < 2) return splits;

  std::vector<std::string> splitsCleaned;
  splitsCleaned.reserve(splits.size());

  bool lastSkipped = false;
  for (size_t i = 1; i < splits.size(); i++) {
    lastSkipped = false;
    auto toTest = splits[i - 1] + " " + splits[i];
    if (isMultiple(toTest) || isOption(toTest)) {
      splitsCleaned.push_back(toTest);
      lastSkipped = true;
    } else {
      splitsCleaned.push_back(splits[i - 1]);
    }
  }
  if (!lastSkipped) splitsCleaned.push_back(splits[splits.size() - 1]);
  return splitsCleaned;
}

std::vector<std::string> PatternParser::splitOr(const std::string& pattern) {
  bool inStringDouble = false;
  bool inStringSingle = false;
  size_t optionalLevel = 0;
  std::vector<size_t> splitPoints;
  for (size_t i = 0; i < pattern.size(); i++) {
    if (pattern[i] == '[' && !inStringDouble && !inStringSingle) {
      optionalLevel++;
    } else if (pattern[i] == ']' && !inStringDouble && !inStringSingle) {
      if (optionalLevel == 0) {
        throw std::invalid_argument("Unbalanced number or [] in pattern!");
      }
      optionalLevel--;
    } else if (pattern[i] == '\"' && !inStringSingle) {
      inStringDouble = !inStringDouble;
    } else if (pattern[i] == '\'' && !inStringDouble) {
      inStringSingle = !inStringSingle;
    } else if (pattern[i] == '|' && !inStringDouble && !inStringSingle &&
               optionalLevel == 0) {
      splitPoints.push_back(i);
    }
  }
  auto splits = splitAtIndices(pattern, splitPoints);
  for (size_t i = 1; i < splits.size(); i++) {
    splits[i] = splits[i].substr(1);
  }
  return splits;
}

std::vector<std::string> PatternParser::splitAtIndices(
    const std::string& pattern, const std::vector<size_t> splitPoints) {
  std::vector<std::string> splits;
  splits.reserve(splitPoints.size() + 1);
  for (size_t i = 0; i <= splitPoints.size(); i++) {  // Intentional
    size_t start = 0, end = pattern.size();
    if (i != 0) {
      start = splitPoints[i - 1];
    } else if (i != pattern.size()) {
      end = splitPoints[i];
    }
    if (start != end) {
      splits.push_back(pattern.substr(start, end - start));
    }
  }
  return splits;
}

bool PatternParser::isOptional(const std::string& pattern) {
  if (pattern.size() < 3) return false;

  bool inStringDouble = false;
  bool inStringSingle = false;
  size_t optionalLevel = 0;
  int64_t optionalStart = -1;
  int64_t optionalEnd = -1;
  for (size_t i = 0; i < pattern.size(); i++) {
    if (pattern[i] == '[' && !inStringDouble && !inStringSingle) {
      if (optionalLevel == 0) {
        optionalStart = i;
      }
      optionalLevel++;
    } else if (pattern[i] == ']' && !inStringDouble && !inStringSingle) {
      if (optionalLevel == 0) {
        throw std::invalid_argument("Unbalanced number or [] in pattern!");
      }
      optionalLevel--;
      if (optionalLevel == 0) {
        optionalEnd = i;
        break;
      }
    }
  }

  if (inStringDouble) {
    throw std::invalid_argument("Unbalanced number or \" in pattern!");
  } else if (inStringSingle) {
    throw std::invalid_argument("Unbalanced number or \' in pattern!");
  }
  return optionalStart == 0 &&
         optionalEnd == static_cast<int64_t>(pattern.size()) - 1;
}

bool PatternParser::isOption(const std::string& pattern) {
  auto splits = Utilities::splitAtSpacesWithEscape(pattern);
  if (splits.empty()) return false;

  if (splits.size() > 2) return false;
  if (!isValidOptionName(splits[0])) return false;
  if (splits.size() == 2 && !isArgType(splits[1])) return false;
  return true;
}
bool PatternParser::isOrder(const std::string& pattern) {
  auto splits = Utilities::splitAtSpacesWithEscape(pattern);
  if (splits.size() <= 1) return false;
  if (splits.size() == 2) {
    if (splits[1] == "...") return false;
    if (isArgType(splits[1])) return false;
  }
  return (!isOr(pattern) && !isOptional(pattern));
}

bool PatternParser::isOr(const std::string& pattern) {
  bool inOptional = false;
  bool inStringDouble = false;
  bool inStringSingle = false;
  for (size_t i = 0; i < pattern.size(); i++) {
    if (pattern[i] == '[' && !inOptional && !inStringDouble &&
        !inStringSingle) {
      inOptional = true;
    } else if (pattern[i] == ']' && inOptional && !inStringDouble &&
               !inStringSingle) {
      inOptional = false;
    } else if (pattern[i] == '\"' && !inOptional && !inStringSingle) {
      inStringDouble = !inStringDouble;
    } else if (pattern[i] == '\'' && !inOptional && !inStringDouble) {
      inStringSingle = !inStringSingle;
    } else if (pattern[i] == '|' && !inOptional && !inStringDouble &&
               !inStringSingle) {
      return true;
    }
  }
  if (inOptional) {
    throw std::invalid_argument("Unbalanced number or [] in pattern!");
  } else if (inStringDouble) {
    throw std::invalid_argument("Unbalanced number or \" in pattern!");
  } else if (inStringSingle) {
    throw std::invalid_argument("Unbalanced number or \' in pattern!");
  }
  return false;
}
bool PatternParser::isValue(const std::string& pattern) {
  auto splits = Utilities::splitAtSpacesWithEscape(pattern);
  if (splits.empty()) return false;
  if (splits.size() >= 2) return false;
  if (!isValidParamName(splits[0])) return false;
  if (splits.size() == 2 && splits[1] != "...") return false;
  return true;
}

bool PatternParser::isMultiple(const std::string& pattern) {
  auto splits = Utilities::splitAtSpacesWithEscape(pattern);
  if (splits.size() != 2) return false;
  if (!isValidParamName(splits[0]) || splits[1] != "...") return false;
  return true;
}

std::string PatternParser::getValueName(const std::string& pattern) {
  return Utilities::splitAtSpacesWithEscape(pattern)[0];
}

std::pair<std::string, ArgType> PatternParser::getOptionNameAndArgType(
    const std::string& pattern) {
  ArgType out = ArgType::None;
  auto splits = Utilities::splitAtSpacesWithEscape(pattern);
  if (splits.size() == 2) {
    auto argType = Utilities::removeFirstAndLastChar(splits[1]);
    out = toArgType(argType);
  }
  return {splits[0], out};
}

std::string PatternParser::getMultipleName(const std::string& pattern) {
  return Utilities::splitAtSpacesWithEscape(pattern)[0];
}

}  // namespace ConsoleUtils
