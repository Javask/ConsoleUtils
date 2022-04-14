#include "PatternTokenizer.h"
#include "StringUtils.h"
#include <stdexcept>

const std::regex PatternTokenizer::paramNameRegex =
    std::regex("[[:alpha:]][\\w_-]*");
const std::regex PatternTokenizer::optionNameRegex =
    std::regex("-+\\w[\\w_-]*");

// TODO Refactor this mess
std::vector<PatternToken> PatternTokenizer::tokenize(
    const std::string& pattern) {
  auto outVector = std::vector<PatternToken>();
  auto splits = splitAtSpaces(pattern);

  bool inOptional = false;
  PatternToken* previousToken = nullptr;
  for (size_t i = 0; i < splits.size(); i++) {
    auto token = PatternToken();
    auto split = splits[i];

    // Check if we start a segment and remove braces
    auto startOptional = (split[0] == '[');
    if (startOptional) split = removeFirstChar(split);
    auto endOptional = (split[split.size() - 1] == ']');
    if (endOptional) split = removeLastChar(split);

    // Enforce [] consistency
    if (inOptional) {
      if (startOptional) {
        throw std::invalid_argument("Nesting [ in argument " +
                                    std::to_string(i) + ".");
      }
      if (!endOptional) {
        throw std::invalid_argument("Missing ] in argument " +
                                    std::to_string(i) + ".");
      }
    } else if (endOptional && !startOptional) {
      throw std::invalid_argument("Excessive ] in argument " +
                                  std::to_string(i) + ".");
    }

    // Handle multiples
    if (split == "...") {
      if (!previousToken) {
        throw std::invalid_argument("... as first argument!");
      }
      previousToken->canBeMultiple = true;
    } else {
      // Handle arg_types
      if (isArgType(split)) {
        if (!inOptional || !previousToken || !previousToken->option) {
          throw std::invalid_argument("Unexpected arg_type in argument " +
                                      std::to_string(i) + ".");
        }
        split = removeFirstAndLastChar(split);
        auto argType = toArgType(split);
        if (argType == ArgType::None) {
          throw std::invalid_argument("Invalid arg_type " + split +
                                      " at argument " + std::to_string(i) +
                                      ".");
        }
        previousToken->argtype = argType;
      } else {
        // If we are not ... or an arg type we cannot be in an ongoing optional!
        if (inOptional) {
          throw std::invalid_argument(
              "Expected ... or arg_type to close token at argument " +
              std::to_string(i) + " !");
        }

        // Handle optional and options
        if (startOptional) {
          token.isOptional = true;
        }
        if (isOption(split)) {
          if (!startOptional) {
            throw std::invalid_argument(
                "Non optional option detected at argument " +
                std::to_string(i) + " !");
          }
          token.option = true;
        }

        // Check name
        if (token.option) {
          if (!isValidOptionName(split)) {
            throw std::invalid_argument("Invalid option " + split +
                                        " at argument " + std::to_string(i) +
                                        ".");
          }
        } else {
          if (!isValidParamName(split)) {
            throw std::invalid_argument("Invalid argument " + split +
                                        " at argument " + std::to_string(i) +
                                        ".");
          }
        }

        // Handle indistinguishable tokens
        if (previousToken && previousToken->canBeMultiple) {
          throw std::invalid_argument("Cannot distiguish token " +
                                      std::to_string(i) +
                                      "from ... values before it!");
        }

        // Push new token
        token.name = split;
        outVector.push_back(token);
      }
    }
    previousToken = &outVector[outVector.size() - 1];
    inOptional = (startOptional && !endOptional);
  }
  return outVector;
}

bool PatternTokenizer::isValidOptionName(const std::string& val) {
  return std::regex_match(val, optionNameRegex);
}

bool PatternTokenizer::isValidParamName(const std::string& val) {
  return std::regex_match(val, paramNameRegex);
}

bool PatternTokenizer::isOptional(const std::string& val) {
  return val.size() >= 3 && val[0] == '[' && val[val.size() - 1] == ']';
}

bool PatternTokenizer::isOption(const std::string& val) {
  if (!(val.size() >= 2 && val[0] == '-')) return false;
  for (size_t i = 1; i < val.size(); i++) {
    if (val[i] != '-') return true;
  }
  return false;
}

bool PatternTokenizer::isArgType(const std::string& val) {
  return val.size() >= 3 && val[0] == '<' && val[val.size() - 1] == '>';
}
