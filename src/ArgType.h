#pragma once
#include <string>

enum class ArgType {
  String,
  Real,
  PositiveReal,
  Integer,
  PositiveInteger,
  Boolean,
  None
};

ArgType toArgType(const std::string& val);