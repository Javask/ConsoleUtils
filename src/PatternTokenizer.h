#pragma once
#include <string>
#include <regex>
#include "PatternToken.h"

/* Pattern definition:
  Names may not contain spaces!
  name Required parameter
  [name] One optional parameter
  name ... At least one parameter
  [name ...] More than one optional parameters
  [--name] named option
  [--name <arg_type>] named option with value type arg_type
  arg_types: string, [+]real, [+]int, bool

Notes:
  arg_type string: cannot contain spaces unless surrounded in quotes
  arg_type bool: accept values true/false, ignore case!
  arg_type int/real: [+] restricts to positive values
  --help and -? always implemented!, print pattern and all params in order,
  with descriptions
*/

class PatternTokenizer {
 private:
  PatternTokenizer() = default;
  ~PatternTokenizer() = default;

 public:
  static std::vector<PatternToken> tokenize(const std::string& pattern);

 private:
  static bool isOptional(const std::string& val);
  static bool isOption(const std::string& val);

  static bool isArgType(const std::string& val);

  static bool isValidParamName(const std::string& val);
  static bool isValidOptionName(const std::string& val);

  static const std::regex paramNameRegex;
  static const std::regex optionNameRegex;
};