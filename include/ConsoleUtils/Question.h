#pragma once
#include <optional>
#include <string>
#include <istream>
#include <ostream>
namespace ConsoleUtils {
class Question {
 public:
  Question();
  Question(std::istream* cin, std::ostream* cout);
  ~Question() = default;

  std::string askString(const std::string& question);
  std::optional<double> askNumber(const std::string& question,
                                  bool repeat = false);
  std::optional<double> askNumber(const std::string& question, double from,
                                  double to, bool repeat = false);
  enum YesNoMode { YesDefault, NoDefault, Neither };
  std::optional<bool> askYesNo(const std::string& question,
                               YesNoMode mode = Neither, bool repeat = false);

 private:
  std::istream* cin;
  std::ostream* cout;
};
}  // namespace ConsoleUtils