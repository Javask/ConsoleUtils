#pragma once
#include "ParamResult.h"
#include <string>
#include <vector>
#include <optional>

struct ParamDescription {
  std::string name;
  std::string description;
};

typedef std::string ParamPattern;

class ParamParser {
 public:
  ParamParser(ParamPattern pattern, std::vector<ParamDescription> params);
  ~ParamParser() = default;

  ParamResult parse(int argc, char** argv);

 private:
  void printHelp();
  ParamPattern pattern;
  std::vector<ParamDescription> descriptions;
};