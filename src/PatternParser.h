#pragma once
#include "PatternToken.h"
#include <stack>
namespace ConsoleUtils {

enum class StringPartType {
  Value,    //  a
  Option,   // -a
  ArgType,  // <a>
  Multiple  // ...
};

enum class LayerType {
  Or,       // a | b | c
  Order,    // (a b c) , a b c
  Optional  // [a b c]
};

PatternTokenType toPatternTokenType(LayerType type);
PatternTokenType toPatternTokenType(StringPartType type);

struct StringPartInfo {
  StringPartType type;
  int64_t layer = -1;
  int64_t start = -1;
  int64_t end = -1;
  bool containsMultiple = false;
  bool containsArgType = false;
};

struct LayerInfo {
  LayerType type;
  int64_t start = -1;
  int64_t end = -1;
  int64_t parent = -1;
  std::vector<size_t> stringPartIds;
};

class LoopState {
 public:
  LoopState() = default;
  ~LoopState() = default;

  const std::vector<StringPartInfo>& getStringParts() const;
  const std::vector<LayerInfo>& getLayers() const;
  const std::vector<std::vector<PatternTokenType>> getCharInfo() const;

  void handle(const std::string& pattern);
  void reset();

 private:
  bool wasEscape = false;
  bool wasSpace = false;
  int64_t lastSpace = -1;
  int64_t lastEscape = -1;
  std::stack<size_t> currLayerId;
  std::stack<size_t> currStringPartId;
  std::vector<StringPartInfo> stringParts;
  std::vector<LayerInfo> layers;
  std::vector<std::vector<PatternTokenType>> charInfo;

  bool inStringPart(StringPartType type);
  void addLayer(LayerType type, size_t start);
  void endLayer(LayerType type, size_t end);
  void beginStringPart(StringPartType type, size_t start);
  void endStringPart(StringPartType type, size_t end);

  bool inValue();
  bool inOption();
  bool inArgType();
  bool inMultiple();

  void addLayerOptional(size_t start);
  void addLayerBrackets(size_t start);
  void endLayerOptional(size_t end);
  void endLayerBrackets(size_t end);

  void insertOrLayer(size_t pos);

  void beginValue(size_t start);
  void beginOption(size_t start);
  void beginArgType(size_t start);
  void beginMultiple(size_t start);

  void endValue(size_t end);
  void endOption(size_t end);
  void endArgType(size_t end);
  void endMultiple(size_t end);
  void endStringPartNotArgType(size_t end);

  void handleTokenEnd(const std::string& pattern, size_t i);
  void checkSpace(const std::string& pattern, size_t i);
  void checkEscape(const std::string& pattern, size_t i);
  void checkOptional(const std::string& pattern, size_t i);
  void checkBrackets(const std::string& pattern, size_t i);
  void checkOr(const std::string& pattern, size_t i);
  void checkValue(const std::string& pattern, size_t i);
  void checkOption(const std::string& pattern, size_t i);
  void checkArgType(const std::string& pattern, size_t i);
  void fixMultipleAndArgType();
  void setupCharInfo(const std::string& pattern);
};

class PatternParser {
 private:
  PatternParser() = default;
  ~PatternParser() = default;

 public:
  static std::shared_ptr<PatternToken> parse(const std::string& pattern);

 private:
};

}  // namespace ConsoleUtils