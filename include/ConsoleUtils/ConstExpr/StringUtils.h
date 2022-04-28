#pragma once
#include <string>
#include <array>
#include <cstddef>

namespace ConsoleUtils::ConstExpr::String {
template <size_t count>
constexpr size_t getSeqmentCount(const char str[count]) {
  size_t out = 0;
  bool wasSpace = false;
  for (size_t i = 0; i < count; i++) {
    char curr = str[i];
    if (curr != ' ' && wasSpace && curr != '\0') {
      out++;
    }
    wasSpace = (curr == ' ');
  }
  return out;
}

template <size_t count, const char str[count]>
constexpr std::array<std::basic_string<char>, getSeqmentCount<count>(str)>
splitAtSpaces() {
  auto out = std::array<std::basic_string<char>, getSeqmentCount<count>(str)>();
  size_t index = 0;
  bool wasSpace = false, encounteredNonSpace = false;
  for (size_t i = 0; i < count; i++) {
    char curr = str[i];
    if (curr != ' ' && curr != '\0') {
      if (wasSpace && encounteredNonSpace) index++;
      out[index] += curr;
    }
    encounteredNonSpace = (curr != ' ' || encounteredNonSpace);
    wasSpace = (curr == ' ');
  }
  return out;
}

};  // namespace ConsoleUtils::ConstExpr::String