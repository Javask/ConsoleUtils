#include "ConsoleUtils/ConsoleInterface.h"
#include <iostream>
#include <Utilities/StringUtils.h>
namespace ConsoleUtils {
ConsoleInterface::ConsoleInterface()
    : ConsoleInterface(&std::cin, &std::cout) {}

ConsoleInterface::ConsoleInterface(std::istream* in, std::ostream* out)
    : cin(in), cout(out) {
  ConsoleCommand exitCommand;
  exitCommand.helpDescription = "Exit the program";
  exitCommand.pattern = "exit";
  exitCommand.onExec = [this](std::string, std::istream&, std::ostream&) {
    handleExit();
  };
  cmds["exit"] = exitCommand;

  ConsoleCommand helpCommand;
  helpCommand.helpDescription = "Print this help dialogue.";
  helpCommand.pattern = "help";
  helpCommand.onExec = [this](std::string, std::istream&, std::ostream&) {
    handleHelp();
  };
  cmds["help"] = helpCommand;
}

void ConsoleInterface::registerCommand(ConsoleCommand cmd) {
  auto splits = Utilities::splitAtSpacesWithEscape(cmd.pattern);
  if (splits.size() != 0) cmds[splits[0]] = cmd;
}

void ConsoleInterface::unregisterCommand(std::string name) {
  auto splits = Utilities::splitAtSpacesWithEscape(name);
  if (splits.size() != 0) cmds.erase(splits[0]);
}

void ConsoleInterface::enableCommand(std::string name) {
  auto splits = Utilities::splitAtSpacesWithEscape(name);
  if (splits.size() != 0 && cmds.contains(splits[0]))
    cmds[splits[0]].enabled = true;
}

void ConsoleInterface::disableCommand(std::string name) {
  auto splits = Utilities::splitAtSpacesWithEscape(name);
  if (splits.size() != 0 && cmds.contains(splits[0]))
    cmds[splits[0]].enabled = false;
}

void ConsoleInterface::run() {
  constexpr size_t bufferSize = 256;
  char buffer[bufferSize] = "";
  std::string currLine = "";
  while (exit.load() == false) {
    if (!cin->fail()) {
      *cout << "> ";
    }

    cin->getline(buffer, bufferSize);

    if (!cin->fail()) {
      currLine = std::string(buffer);
      handleLine(currLine);
    }
  }
}

void ConsoleInterface::handleLine(const std::string& line) {
  auto splits = Utilities::splitAtSpacesWithEscape(line);
  if (splits.size() != 0 && cmds.contains(splits[0])) {
    cmds[splits[0]].onExec(line, *cin, *cout);
  } else {
    handleUnknown();
  }
}

void ConsoleInterface::handleUnknown() {
  *cout
      << "Unknown command!\n Use \"help\" for a list of available commands.\n";
}

void ConsoleInterface::handleHelp() {
  size_t padding = 0;
  for (auto& cmd : cmds) {
    if (cmd.second.pattern.size() > padding)
      padding = cmd.second.pattern.size();
  }
  for (auto& cmd : cmds) {
    size_t toPad = padding - cmd.second.pattern.size();
    *cout << cmd.second.pattern << ": ";
    for (size_t i = 0; i < toPad; i++) *cout << " ";
    *cout << cmd.second.helpDescription << "\n";
  }
}

void ConsoleInterface::handleExit() {
  *cout << "Exiting...\n";
  exit.store(true);
}
}  // namespace ConsoleUtils