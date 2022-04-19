#pragma once
#include "PatternToken.h"
#include <vector>
#include <string>
#include <optional>
#include <unordered_map>

// TODO change to throw instead of setError, rename refinematch to compute

typedef std::unordered_map<std::string, std::vector<std::string>> InputTokens;

class InputTokenizer {
 private:
  InputTokenizer() = default;
  ~InputTokenizer() = default;

 public:
  static std::optional<InputTokens> tokenize(
      const std::vector<PatternToken>& pattern, const std::string& input);

 private:
  struct Parameter {
    std::string name;
    bool optional;
    bool multiple;
  };

  static InputTokens& getParameterAssignments(
      const std::vector<Parameter>& parameterTokens,
      const std::vector<std::string>& parameterSplits, InputTokens& output);
  static std::vector<Parameter> getPatternParameters(
      const std::vector<PatternToken>& pattern);
  static std::unordered_map<std::string, ArgType> getPatternOptions(
      const std::vector<PatternToken>& pattern);
  static bool matchesArgType(ArgType type, std::string& split);
  static bool splitIsHelp(std::string& split);
  static std::vector<size_t> getMandatoryParamCounts(
      const std::vector<Parameter>& params);
};
