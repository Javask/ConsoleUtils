#include "ConsoleUtils/ParamParser.h"
#include "PatternTokenizer.h"
#include "InputTokenizer.h"
#include <sstream>
#include <iostream>

ParamParser::ParamParser(ParamPattern pattern,
                         std::vector<ParamDescription> params)
    : pattern(pattern), descriptions(params) {}

ParamResult ParamParser::parse(int argc, char** argv) {
  if (argc < 1) {
    throw std::invalid_argument("argc == 0 should not be possible!");
  }
  std::stringstream stream;
  for (int i = 1; i < argc; i++) {
    stream << argv[i] << " ";
  }
  auto input = stream.str();
  std::vector<PatternToken> patternTokens;
  try {
    patternTokens = PatternTokenizer::tokenize(pattern);
  } catch (std::invalid_argument arg) {
    std::cout << arg.what() << std::endl;
    printHelp();
    throw arg;
  }
  InputTokens tokens;
  try {
    tokens = InputTokenizer::tokenize(patternTokens, input);
  } catch (std::invalid_argument arg) {
    std::cout << arg.what() << std::endl;
    printHelp();
    throw arg;
  }
  if (tokens.contains("--help") || tokens.contains("-?")) {
    printHelp();
    return ParamResult();
  }
  return tokens;
}

void ParamParser::printHelp() {
  // TODO
}
