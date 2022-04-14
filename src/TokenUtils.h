#pragma once
#include <string>

bool isOptional(const std::string& val);
bool isOption(const std::string& val);

bool isArgType(const std::string& val);

bool isValidParamName(const std::string& val);
bool isValidOptionName(const std::string& val);