#pragma once
#include <string>
#include <array>
#include <cstddef>
namespace ConsoleUtils::ConstExpr::Token {

constexpr bool isArgType(const char* val, size_t len) {
  return len >= 3 && val[0] == '<' && val[len - 1] == '>';
}

}  // namespace ConsoleUtils::ConstExpr::Token