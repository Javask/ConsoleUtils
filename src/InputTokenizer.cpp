#include "InputTokenizer.h"
#include <Utilities/StringUtils.h>
#include "TokenUtils.h"
#include <stdexcept>
namespace ConsoleUtils {

bool InputTokenizer::splitIsHelp(std::string& split) {
  return split == "--help" || split == "-?";
}

std::vector<size_t> InputTokenizer::getMandatoryParamCounts(
    const std::vector<Parameter>& params) {
  size_t count = 0;
  std::vector<size_t> counts = std::vector<size_t>(params.size());
  for (size_t i = 0; i < params.size(); i++) {
    counts[i] = count;
    if (!params[i].optional) count++;
  }
  return counts;
}

InputTokens InputTokenizer::tokenize(const std::vector<PatternToken>& pattern,
                                     const std::string& input) {
  InputTokens output;

  auto parameters = getPatternParameters(pattern);
  auto requiredParamCounts = getMandatoryParamCounts(parameters);
  auto options = getPatternOptions(pattern);
  auto splits = Utilities::splitAtSpacesWithEscape(input);

  std::string previousSplit = "";
  ArgType needsArgument = ArgType::None;

  std::vector<std::string> parameterSplits;

  for (size_t i = 0; i < splits.size(); i++) {
    auto split = splits[i];
    if (splitIsHelp(split)) {
      output.clear();
      output.emplace(split, std::vector<std::string>());
      return output;
    }
    if (options.contains(split)) {
      auto& opt = options[split];
      output.emplace(split, std::vector<std::string>());
      if (opt != ArgType::None) {
        needsArgument = opt;
      }
    } else if (needsArgument != ArgType::None) {
      if (matchesArgType(needsArgument, split)) {
        output[previousSplit].push_back(split);
        needsArgument = ArgType::None;
      } else {
        throw std::invalid_argument("Invalid argument for option " +
                                    previousSplit + "!");
      }
    } else {
      parameterSplits.push_back(split);
    }
    previousSplit = split;
  }
  if (needsArgument != ArgType::None) {
    throw std::invalid_argument("No argument found for option " +
                                previousSplit + "!");
  }
  for (auto& paramSplit : parameterSplits) {
    if (isValidOptionName(paramSplit))
      throw std::invalid_argument("Invalid option " + paramSplit + " !");
  }

  output = getParameterAssignments(parameters, parameterSplits, output);
  return output;
}

InputTokens& InputTokenizer::getParameterAssignments(
    const std::vector<Parameter>& parameterTokens,
    const std::vector<std::string>& parameterSplits, InputTokens& output) {
  auto usedParameters = std::vector<Parameter>();
  for (auto& paramToken : parameterTokens) {
    if (!paramToken.optional) usedParameters.push_back(paramToken);
  }

  if (usedParameters.size() > parameterSplits.size())
    throw std::invalid_argument("Not enough parameters!");
  for (size_t i = 0; i < parameterTokens.size() &&
                     usedParameters.size() != parameterSplits.size();
       i++) {
    auto currToken = parameterTokens[i];
    if (currToken.optional && !currToken.multiple) {
      usedParameters.insert(usedParameters.begin() + i, currToken);
    } else if (currToken.optional) {
      size_t diff = parameterSplits.size() - usedParameters.size();
      for (size_t j = 0; j < diff; j++) {
        usedParameters.insert(usedParameters.begin() + i, currToken);
      }
    }
  }
  if (usedParameters.size() != parameterSplits.size())
    throw std::invalid_argument("Not enough parameters!");

  for (size_t i = 0; i < usedParameters.size(); i++) {
    output[usedParameters[i].name].push_back(parameterSplits[i]);
  }
  return output;
}

std::vector<InputTokenizer::Parameter> InputTokenizer::getPatternParameters(
    const std::vector<PatternToken>& pattern) {
  std::vector<InputTokenizer::Parameter> parameters;
  bool hasMultiple = false;
  for (size_t i = 0; i < pattern.size(); i++) {
    auto& currToken = pattern[i];
    if (!currToken.option) {
      Parameter param;
      param.multiple = currToken.canBeMultiple;
      param.name = currToken.name;
      param.optional = currToken.isOptional;

      if (hasMultiple && (currToken.canBeMultiple || currToken.isOptional))
        throw std::invalid_argument("Indistinguishable parameters in pattern!");

      if (currToken.canBeMultiple) hasMultiple = true;

      parameters.push_back(param);
    }
  }
  return parameters;
}

std::unordered_map<std::string, ArgType> InputTokenizer::getPatternOptions(
    const std::vector<PatternToken>& pattern) {
  std::unordered_map<std::string, ArgType> options;
  for (size_t i = 0; i < pattern.size(); i++) {
    auto& currToken = pattern[i];
    if (currToken.option) {
      options.emplace(currToken.name, currToken.argtype);
    }
  }
  return options;
}

bool InputTokenizer::matchesArgType(ArgType type, std::string& split) {
  auto toLower = Utilities::toLowerCase(split);
  switch (type) {
    case ArgType::None:
      return false;
    case ArgType::String:
      return true;
    case ArgType::Integer:
      return Utilities::isInteger(split);
    case ArgType::Real:
      return Utilities::isReal(split);
    case ArgType::PositiveInteger:
      return Utilities::isInteger(split, false);
    case ArgType::PositiveReal:
      return Utilities::isReal(split, false);
    case ArgType::Boolean:
      return toLower == "false" || toLower == "true";
    default:
      return false;
  }
}
}  // namespace ConsoleUtils
