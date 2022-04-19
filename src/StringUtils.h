#pragma once
#include <string>
#include <vector>

std::vector<std::string> splitAtSpaces(const std::string& val);
std::vector<std::string> splitAtSpacesWithEscape(const std::string& val);
std::string removeFirstAndLastChar(const std::string& val);
std::string removeFirstChar(const std::string& val);
std::string removeLastChar(const std::string& val);
std::string toLowerCase(const std::string& val);
bool isInteger(const std::string& val, bool allowNegative = true);
bool isReal(const std::string& val, bool allowNegative = true);