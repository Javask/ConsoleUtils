#include "ArgType.h"
ArgType toArgType(const std::string& val) {
  if (val == "string") {
    return ArgType::String;
  } else if (val == "int") {
    return ArgType::Integer;
  } else if (val == "real") {
    return ArgType::Real;
  } else if (val == "+int") {
    return ArgType::PositiveInteger;
  } else if (val == "+real") {
    return ArgType::PositiveReal;
  } else if (val == "bool") {
    return ArgType::Boolean;
  } else {
    return ArgType::None;
  }
}
