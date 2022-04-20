#pragma once
#include <filesystem>
class Discovery {
 private:
  Discovery() = default;
  ~Discovery() = default;

  static bool isSet;
  static std::filesystem::path executablePath;

 public:
  static void resetExecutablePath();
  static void overrideExecutablePath(const std::filesystem::path& path);
  // Returns the Path to the running executable
  static auto getExecutablePath() -> std::filesystem::path;
};
