#include "Discovery.h"
#ifdef CONSOLEUTILS_PLATFORM_WINDOWS
#include <Windows.h>
#else
#include <linux/limits.h>
#include <unistd.h>
#endif
#include <vector>
#include <iostream>

bool Discovery::isSet = false;

std::filesystem::path Discovery::executablePath = std::filesystem::path();

void Discovery::overrideExecutablePath(const std::filesystem::path& path) {
  executablePath = path;
  isSet = true;
}

void Discovery::resetExecutablePath() { isSet = false; }

std::filesystem::path Discovery::getExecutablePath() {
  if (isSet) return executablePath;

#ifdef CONSOLEUTILS_PLATFORM_WINDOWS
  // Use Windows API to find Current file
  std::vector<char> FileName = std::vector<char>(128);
  while (GetModuleFileNameA(nullptr, FileName.data(), (DWORD)FileName.size()) ==
         (DWORD)FileName.size()) {
    if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
      std::cerr << "Could not get filename!";
      abort();  // Unknown Error while finding executable
    }
    FileName.resize(FileName.size() + 128);
  }
  return {FileName.data()};
#else
  // Use Linux proc directory to find Current file
  // TODO Implement other OSes when targeted
  // TODO Bounds check count to PATH_MAX
  char FileName[PATH_MAX] = {};
  ssize_t count = readlink("/proc/self/exe", FileName, PATH_MAX);
  return std::string(FileName, (count > 0) ? count : 0);
#endif
}
