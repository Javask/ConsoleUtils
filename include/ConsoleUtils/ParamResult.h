#pragma once
#include <unordered_map>
#include <string>
#include <vector>

typedef std::unordered_map<std::string, std::vector<std::string>>
    ParameterValues;

class ParamResult {
 public:
  static ParamResult create(ParameterValues values);
  static ParamResult createResultShouldExit();
  ~ParamResult() = default;
  ParameterValues getValues();
  bool shouldExit();

 private:
  ParamResult(ParameterValues values, bool exit);
  ParameterValues values;
  bool exit;
};
