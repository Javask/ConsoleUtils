#pragma once
#include <istream>
#include <ostream>
#include <functional>
#include <string>
#include <map>
#include <atomic>

struct ConsoleCommand {
  std::string pattern;
  std::string helpDescription;
  std::function<void(std::string, std::istream&, std::ostream&)> onExec;
  bool enabled = true;
};

class ConsoleInterface {
 public:
  ConsoleInterface();
  ConsoleInterface(std::istream* in, std::ostream* out);

  void registerCommand(ConsoleCommand cmd);
  void unregisterCommand(std::string name);

  void enableCommand(std::string name);
  void disableCommand(std::string name);

  void run();

 private:
  void handleLine(const std::string& line);
  void handleUnknown();
  void handleHelp();
  void handleExit();

  std::istream* cin = nullptr;
  std::ostream* cout = nullptr;

  std::atomic_bool exit = false;
  std::map<std::string, ConsoleCommand> cmds;
};