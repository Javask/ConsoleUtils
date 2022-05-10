#include "PatternParser.h"
#include <Utilities/StringUtils.h>
#include "TokenUtils.h"
#include <algorithm>

namespace ConsoleUtils {
PatternTokenType toPatternTokenType(LayerType type) {
  switch (type) {
    case LayerType::Optional:
      return OPTIONAL;
    case LayerType::Or:
      return OR;
    case LayerType::Order:
      return ORDER;
  }
}

PatternTokenType toPatternTokenType(StringPartType type) {
  switch (type) {
    case StringPartType::ArgType:
    case StringPartType::Multiple:
      throw std::invalid_argument(
          "ArgTypes and Multiples should have been removed by this point!");
    case StringPartType::Option:
      return OPTION;
    case StringPartType::Value:
      return VALUE;
  }
}

std::shared_ptr<PatternToken> PatternParser::parse(const std::string& pattern) {
  LoopState state;
  state.handle(pattern);

  throw std::invalid_argument("Could not parse pattern \"" + pattern + "\"");
}

const std::vector<std::vector<PatternTokenType>> LoopState::getCharInfo()
    const {
  return charInfo;
}

bool LoopState::inStringPart(StringPartType type) {
  return !stringParts.empty() && (stringParts.back().type == type);
}

bool LoopState::inValue() { return inStringPart(StringPartType::Value); }
bool LoopState::inOption() { return inStringPart(StringPartType::Option); }
bool LoopState::inArgType() { return inStringPart(StringPartType::ArgType); }
bool LoopState::inMultiple() { return inStringPart(StringPartType::Multiple); }

void LoopState::addLayer(LayerType type, size_t start) {
  LayerInfo newLayer = {};
  newLayer.type = type;
  newLayer.start = start;
  if (!currLayerId.empty()) newLayer.parent = currLayerId.top();
  size_t id = layers.size();
  layers.push_back(newLayer);
  currLayerId.push(id);
}
void LoopState::addLayerOptional(size_t start) {
  addLayer(LayerType::Optional, start);
}
void LoopState::addLayerBrackets(size_t start) {
  addLayer(LayerType::Order, start);
}

void LoopState::insertOrLayer(size_t pos) {
  auto oldId = currLayerId.top();
  auto& oldLayer = layers[oldId];
  oldLayer.end = pos;
  currLayerId.pop();

  if (oldLayer.parent == -1 || layers[oldLayer.parent].type != LayerType::Or) {
    auto newId = layers.size();
    LayerInfo newLayer = {};
    newLayer.type = LayerType::Or;
    newLayer.parent = oldLayer.parent;
    oldLayer.parent = newId;
    newLayer.start = oldLayer.start;
    layers.push_back(newLayer);
    currLayerId.push(newId);
  }
  addLayer(LayerType::Order, pos);
}

void LoopState::endLayer(LayerType type, size_t end) {
  if (currLayerId.empty()) {
    throw std::invalid_argument(
        "Found closing bracked without matching opening bracket!");
  }
  auto currId = currLayerId.top();
  if (layers[currId].type != type) {
    throw std::invalid_argument("Invalid combination of brackets!");
  }
  layers[currId].end = end;
  currLayerId.pop();

  auto parentId = layers[currId].parent;
  if (parentId != -1) {
    auto& parentLayer = layers[parentId];
    if (parentLayer.type == LayerType::Or) {
      endLayer(LayerType::Or, end);
    }
  }
}
void LoopState::endLayerOptional(size_t end) {
  endLayer(LayerType::Optional, end);
}
void LoopState::endLayerBrackets(size_t end) {
  endLayer(LayerType::Order, end);
}

void LoopState::beginStringPart(StringPartType type, size_t start) {
  size_t id = stringParts.size();
  StringPartInfo sInfo = {};
  sInfo.type = type;
  sInfo.start = start;
  if (!currLayerId.empty()) {
    sInfo.layer = currLayerId.top();
    layers[currLayerId.top()].stringPartIds.push_back(id);
  }
  stringParts.push_back(sInfo);
  currStringPartId.push(id);
}
void LoopState::beginValue(size_t start) {
  beginStringPart(StringPartType::Value, start);
}
void LoopState::beginOption(size_t start) {
  beginStringPart(StringPartType::Option, start);
}
void LoopState::beginArgType(size_t start) {
  beginStringPart(StringPartType::ArgType, start);
}
void LoopState::beginMultiple(size_t start) {
  beginStringPart(StringPartType::Multiple, start);
}

void LoopState::endStringPart(StringPartType type, size_t end) {
  auto currId = currStringPartId.top();
  if (stringParts[currId].type != type) {
    throw std::invalid_argument("Invalid pattern!");
  }
  stringParts[currId].end = end;
  currStringPartId.pop();
}
void LoopState::endValue(size_t end) {
  endStringPart(StringPartType::Value, end);
}
void LoopState::endOption(size_t end) {
  endStringPart(StringPartType::Option, end);
}
void LoopState::endArgType(size_t end) {
  endStringPart(StringPartType::ArgType, end);
}
void LoopState::endMultiple(size_t end) {
  endStringPart(StringPartType::Multiple, end);
}

void LoopState::endStringPartNotArgType(size_t end) {
  auto currId = currStringPartId.top();
  if (stringParts[currId].type == StringPartType::ArgType) {
    throw std::invalid_argument("Invalid pattern!");
  }
  stringParts[currId].end = end;
  currStringPartId.pop();
}
void LoopState::handleTokenEnd(const std::string&, size_t i) {
  endStringPartNotArgType(i);
}

void LoopState::checkSpace(const std::string& pattern, size_t i) {
  if (pattern[i] == ' ' && !wasEscape) {
    handleTokenEnd(pattern, i);
    lastSpace = (int64_t)i;
  }
}

void LoopState::checkEscape(const std::string& pattern, size_t i) {
  if (pattern[i] == '\\' && !wasEscape) {
    lastEscape = (int64_t)i;
  }
}

void LoopState::checkOptional(const std::string& pattern, size_t i) {
  if (pattern[i] == '[' && !wasEscape) {
    handleTokenEnd(pattern, i);
    addLayerOptional(i);
  } else if (pattern[i] == ']' && !wasEscape) {
    handleTokenEnd(pattern, i);
    endLayerOptional(i);
  }
}
void LoopState::checkBrackets(const std::string& pattern, size_t i) {
  if (pattern[i] == '(' && !wasEscape) {
    handleTokenEnd(pattern, i);
    addLayerBrackets(i);
  } else if (pattern[i] == ')' && !wasEscape) {
    handleTokenEnd(pattern, i);
    endLayerBrackets(i);
  }
}

void LoopState::checkOr(const std::string& pattern, size_t i) {
  if (pattern[i] == '|' && !wasEscape) {
    handleTokenEnd(pattern, i);
    addLayerBrackets(i);
  } else if (pattern[i] == ')' && !wasEscape) {
    handleTokenEnd(pattern, i);
    endLayerBrackets(i);
  }
}

void LoopState::checkValue(const std::string& pattern, size_t i) {
  if (pattern[i] == ' ' || pattern[i] == '[' || pattern[i] == ']' ||
      pattern[i] == '(' || pattern[i] == ')' || pattern[i] == '\'' ||
      pattern[i] == '\"')
    return;
  if (inValue() || inOption() || inArgType()) return;
  if (pattern[i] != '-' && pattern[i] != '<') {
    beginValue(i);
  }
}
void LoopState::checkOption(const std::string& pattern, size_t i) {
  if (pattern[i] == ' ' || pattern[i] == '[' || pattern[i] == ']' ||
      pattern[i] == '(' || pattern[i] == ')' || pattern[i] == '\'' ||
      pattern[i] == '\"')
    return;
  if (inValue() || inOption() || inArgType()) return;
  if (pattern[i] == '-' && pattern[i] != '<') {
    beginOption(i);
  }
}

void LoopState::checkArgType(const std::string& pattern, size_t i) {
  if (inValue() || inOption() || inArgType()) return;
  if (pattern[i] == '<') {
    beginArgType(i);
  } else if (pattern[i] != '>') {
    endArgType(i);
  }
}

void LoopState::handle(const std::string& pattern) {
  if (pattern.size() == 0) return;

  for (size_t i = 0; i < pattern.size(); i++) {
    wasEscape = (lastEscape != -1 && (int64_t)i - 1 == lastEscape);
    wasSpace = (lastSpace != -1 && (int64_t)i - 1 == lastSpace);
    checkSpace(pattern, i);
    checkEscape(pattern, i);
    checkOptional(pattern, i);
    checkBrackets(pattern, i);
    checkValue(pattern, i);
    checkOption(pattern, i);
    checkArgType(pattern, i);
    checkOr(pattern, i);
  }
  fixMultipleAndArgType();
  setupCharInfo(pattern);
}

void LoopState::setupCharInfo(const std::string& pattern) {
  charInfo.clear();
  charInfo.resize(pattern.size());
  for (size_t j = 0; j < stringParts.size(); j++) {
    auto& currPart = stringParts[j];
    std::stack<PatternTokenType> typeHierarchy;
    typeHierarchy.push(toPatternTokenType(currPart.type));
    if (currPart.layer != -1) {
      auto& currLayer = layers[currPart.layer];
      typeHierarchy.push(toPatternTokenType(currLayer.type));
      while (currLayer.parent != -1) {
        currLayer = layers[currLayer.parent];
        typeHierarchy.push(toPatternTokenType(currLayer.type));
      }
    }
    int64_t start = currPart.start, end = currPart.end;
    if (start == -1) start = 0;
    if (end == -1) end = pattern.size() - 1;
    while (!typeHierarchy.empty()) {
      auto topType = typeHierarchy.top();
      typeHierarchy.pop();
      for (int64_t i = start; i <= end; i++) {
        charInfo[i].push_back(topType);
      }
    }
  }
}

void LoopState::fixMultipleAndArgType() {
  std::stack<size_t> toRemove;
  for (size_t i = 0; i < stringParts.size(); i++) {
    auto& currPart = stringParts[i];
    if (currPart.type != StringPartType::ArgType &&
        currPart.type != StringPartType::Multiple)
      continue;

    StringPartInfo* beforePart = nullptr;
    for (size_t j = 0; j < stringParts.size(); j++) {
      if (j == i) continue;
      if (stringParts[j].layer != currPart.layer) continue;
      if (stringParts[j].end == currPart.start) {
        if (beforePart != nullptr ||
            (stringParts[j].type != StringPartType::Option &&
             currPart.type == StringPartType::ArgType) ||
            (stringParts[j].type != StringPartType::Value &&
             currPart.type == StringPartType::Multiple)) {
          throw std::invalid_argument("Found misplaced argtype/multiple!");
        }
        beforePart = &stringParts[j];
      }
    }
    if (!beforePart) {
      throw std::invalid_argument("Found misplaced argtype/multiple!");
    }
    beforePart->end = currPart.end;
    beforePart->containsMultiple = (currPart.type == StringPartType::Multiple);
    beforePart->containsArgType = (currPart.type == StringPartType::ArgType);
    toRemove.push(i);
  }
  while (!toRemove.empty()) {
    size_t currRemove = toRemove.top();
    toRemove.pop();

    auto layerId = stringParts[currRemove].layer;
    if (layerId != -1) {
      std::remove_if(layers[layerId].stringPartIds.begin(),
                     layers[layerId].stringPartIds.end(),
                     [currRemove](size_t id) { return id == currRemove; });
    }
    stringParts.erase(stringParts.begin() + currRemove);
  }
}

void LoopState::reset() {
  wasEscape = false;
  wasSpace = false;
  lastSpace = -1;
  lastEscape = -1;
  currLayerId = std::stack<size_t>();
  currStringPartId = std::stack<size_t>();
  stringParts.clear();
  layers.clear();
  charInfo.clear();
}
}  // namespace ConsoleUtils
