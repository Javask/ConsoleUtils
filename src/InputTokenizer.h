#pragma once
#include "InputToken.h"
#include "PatternToken.h"
#include <vector>
#include <string>
#include <optional>
#include <unordered_map>

typedef std::unordered_map<std::string, std::vector<std::string>> InputTokens;

class InputTokenizer {
 private:
  InputTokenizer(const std::vector<PatternToken>& tokens);
  ~InputTokenizer() = default;

 public:
  static InputTokens tokenize(const std::string& input,
                              std::vector<PatternToken>& pattern);
  std::string getError();

 private:
  std::optional<PatternToken> matchNextToken(const std::string& input);
  const std::vector<PatternToken> tokens;
};
