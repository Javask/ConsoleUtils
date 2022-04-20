#pragma once
#include "ParamResult.h"
#include <string>
#include <vector>
#include <optional>
#include <ostream>
#include <iostream>

struct ParamDescription {
  std::string name;
  std::string description;
};

typedef std::string ParamPattern;

class ParamParser {
 public:
  ParamParser(ParamPattern pattern, std::vector<ParamDescription> params);
  ~ParamParser() = default;
  void setOutput(std::ostream* os);
  ParamResult parse(int argc, char** argv);

 private:
  void printException(std::exception& e, const std::string& arg0);
  void printHelp(const std::string& arg0);
  ParamPattern pattern;
  std::vector<ParamDescription> descriptions;
  std::ostream* console = &std::cout;
};