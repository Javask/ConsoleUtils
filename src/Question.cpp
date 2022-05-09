#include "ConsoleUtils/Question.h"
#include <iostream>
#include <Utilities/StringUtils.h>

namespace ConsoleUtils {

Question::Question() : Question(&std::cin, &std::cout) {}

Question::Question(std::istream* cin, std::ostream* cout)
    : cin(cin), cout(cout) {}

std::string Question::askString(const std::string& question) {
  if (!cin) throw std::runtime_error("Invalid input stream!");
  if (!cout) throw std::runtime_error("Invalid output stream!");

  *cout << question << ": ";
  std::string ret = "";
  std::getline(*cin, ret);
  return ret;
}

std::optional<double> Question::askNumber(const std::string& question,
                                          bool repeat) {
  do {
    auto val = askString(question);
    try {
      return std::stod(val);
    } catch (std::invalid_argument&) {
    }
  } while (repeat);
  return {};
}

std::optional<double> Question::askNumber(const std::string& question,
                                          double from, double to, bool repeat) {
  std::optional<double> val = {};
  std::string nQuestion =
      question + "(" + std::to_string(from) + " - " + std::to_string(to) + ")";
  do {
    val = askNumber(nQuestion, repeat);
    if (val) {
      if (*val >= from && *val <= to) return val;
    }
  } while (repeat);
  return val;
}

std::optional<bool> Question::askYesNo(const std::string& question,
                                       YesNoMode mode, bool repeat) {
  std::string nQuestion = question;
  switch (mode) {
    case YesNoMode::YesDefault:
      nQuestion += " (Y/n)";
      break;
    case YesNoMode::NoDefault:
      nQuestion += " (y/N)";
      break;
    default:
      nQuestion += " (y/n)";
      break;
  }
  do {
    auto val = askString(question);
    if (val.empty()) {
      if (mode == YesDefault) return true;
      if (mode == NoDefault) return false;
    }
    auto lower = Utilities::toLowerCase(val);
    if (lower == "n" || lower == "no")
      return false;
    else if (lower == "y" || lower == "yes")
      return true;
  } while (repeat);
  return {};
}

}  // namespace ConsoleUtils