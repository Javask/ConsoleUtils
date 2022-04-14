#pragma once
#include "ParamResult.h"
#include <regex>

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
};