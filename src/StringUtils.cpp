#include "StringUtils.h"
#include <algorithm>

std::string toLowerCase(const std::string& val) {
  std::string data = val;
  std::transform(data.begin(), data.end(), data.begin(), [](unsigned char c) {
    return static_cast<char>(std::tolower(static_cast<int>(c)));
  });
  return data;
}

std::string removeFirstAndLastChar(const std::string& val) {
  return val.substr(1, val.size() - 2);
}

std::string removeFirstChar(const std::string& val) {
  return val.substr(1, val.size() - 1);
}

std::string removeLastChar(const std::string& val) {
  return val.substr(0, val.size() - 1);
}

std::vector<std::string> splitAtSpaces(const std::string& val) {
  const char space = ' ';
  auto substrings = std::vector<std::string>();
  size_t lastSpace = 0;
  for (size_t i = 0; i < val.size(); i++) {
    if (val[i] == space) {
      if (lastSpace != i) {
        substrings.push_back(val.substr(lastSpace, i - lastSpace));
      }
      lastSpace = i + 1;
    }
  }
  if (lastSpace < val.size()) {
    substrings.push_back(val.substr(lastSpace, val.size() - lastSpace));
  }
  return substrings;
}