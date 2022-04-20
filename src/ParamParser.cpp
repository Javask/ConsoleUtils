#include "ConsoleUtils/ParamParser.h"
#include "PatternTokenizer.h"
#include "InputTokenizer.h"
#include <sstream>
#include <iostream>
#include "TokenUtils.h"
#include "Discovery.h"

ParamParser::ParamParser(ParamPattern pattern,
                         std::vector<ParamDescription> params)
    : pattern(pattern), descriptions(params) {}

void ParamParser::setOutput(std::ostream* os) { console = os; }

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
  } catch (std::invalid_argument& arg) {
    printException(arg);
    throw arg;
  }
  InputTokens tokens;
  try {
    tokens = InputTokenizer::tokenize(patternTokens, input);
  } catch (std::invalid_argument& arg) {
    printException(arg);
    throw arg;
  }
  if (tokens.contains("--help") || tokens.contains("-?")) {
    printHelp();
    return ParamResult::createResultShouldExit();
  }
  return ParamResult::create(tokens);
}

void ParamParser::printException(std::exception& e) {
  if (console) {
    *console << e.what() << "\n";
    printHelp();
  }
}

void ParamParser::printHelp() {
  if (console) {
    auto arg0 = Discovery::getExecutablePath().filename().generic_string();
    *console << "Usage: " << arg0 << " " << pattern;
    size_t optionNameLength = 0;
    size_t paramNameLength = 0;
    std::vector<ParamDescription> options, params;
    for (auto& desc : descriptions) {
      if (isValidOptionName(desc.name)) {
        if (desc.name.size() > optionNameLength)
          optionNameLength = desc.name.size();
        options.push_back(desc);
      } else {
        if (desc.name.size() > paramNameLength)
          paramNameLength = desc.name.size();
        params.push_back(desc);
      }
    }
    if (!params.empty()) {
      *console << "\nParameters:\n";
      for (auto& param : params) {
        size_t padding = paramNameLength - param.name.size();
        *console << "\t" << param.name << ": ";
        for (size_t i = 0; i < padding; i++) *console << " ";
        *console << param.description << "\n";
      }
    }
    if (!options.empty()) {
      *console << "\nOptions:\n";
      for (auto& option : options) {
        size_t padding = optionNameLength - option.name.size();
        *console << "\t" << option.name << ": ";
        for (size_t i = 0; i < padding; i++) *console << " ";
        *console << option.description << "\n";
      }
    }
    *console << std::endl;
  }
}
