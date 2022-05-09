#include "ConsoleUtils/ConsoleInterface.h"
#include "ConsoleUtils/Question.h"
#include <string>

using namespace ConsoleUtils;
int main(void) {
  ConsoleInterface cli;
  Question q;
  ConsoleCommand cmd;
  cmd.helpDescription = "Test Command!";
  cmd.onExec = [](std::string cmd, std::istream&, std::ostream& out) {
    out << "TEst!\n" << cmd << "\n";
  };
  cmd.pattern = "test";

  ConsoleCommand askYN;
  askYN.helpDescription = "Ask yes or no";
  askYN.onExec = [&q](std::string, std::istream&, std::ostream& out) {
    auto v = q.askYesNo("Yes or no please");
    if (v)
      out << *v << "\n";
    else
      out << "No value!\n";
  };
  askYN.pattern = "askYN";

  ConsoleCommand askStr;
  askStr.helpDescription = "Ask a string";
  askStr.onExec = [&q](std::string, std::istream&, std::ostream& out) {
    auto v = q.askString("String please");
    out << v << "\n";
  };
  askStr.pattern = "askS";

  ConsoleCommand askNum;
  askNum.helpDescription = "Ask a number";
  askNum.onExec = [&q](std::string, std::istream&, std::ostream& out) {
    auto v = q.askNumber("Number please");
    if (v)
      out << *v << "\n";
    else
      out << "No value!\n";
  };
  askNum.pattern = "askN";
  cli.registerCommand(cmd);
  cli.registerCommand(askNum);
  cli.registerCommand(askStr);
  cli.registerCommand(askYN);

  cli.run();
  return 0;
}