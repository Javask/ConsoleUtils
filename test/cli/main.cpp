#include "ConsoleUtils/ConsoleInterface.h"

using namespace ConsoleUtils;
int main(void) {
  ConsoleInterface cli;
  ConsoleCommand cmd;
  cmd.helpDescription = "Test Command!";
  cmd.onExec = [](std::string cmd, std::istream&, std::ostream& out) {
    out << "TEst!\n" << cmd << "\n";
  };
  cmd.pattern = "test";
  cli.registerCommand(cmd);

  cli.run();
  return 0;
}