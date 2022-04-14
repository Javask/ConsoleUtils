#include "InputTokenizer.h"
#include "StringUtils.h"
#include "TokenUtils.h"
#include <stdexcept>

InputTokens InputTokenizer::tokenize(const std::string& input,
                                     std::vector<PatternToken>& pattern) {
  InputTokens outTokens;
  auto splits = splitAtSpacesWithEscape(input);
  auto tokenizer = InputTokenizer(pattern);
  for (size_t i = 0; i < splits.size(); i++) {
    auto split = splits[i];
    auto matchedToken = tokenizer.matchNextToken(splits[i]);
    if (!matchedToken) {
      throw std::invalid_argument(tokenizer.getError());
    }
    outTokens[matchedToken->name].push_back(split);
  }
  return outTokens;
}

std::string InputTokenizer::getError() {
  return "";  // TODO implement
}

std::optional<PatternToken> InputTokenizer::matchNextToken(
    const std::string& input) {
  return {};  // TODO implement
}

InputTokenizer::InputTokenizer(const std::vector<PatternToken>& tokens)
    : tokens(tokens) {}
