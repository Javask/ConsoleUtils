#include "ConsoleUtils/SimpleParamParser.h"
#include <sstream>
#include <Utilities/StringUtils.h>
#include <Utilities/FileUtils.h>
#include "TokenUtils.h"
#include <set>

namespace ConsoleUtils {

std::vector<size_t> getOptionSplitIds(const std::vector<std::string>& splits) {
  std::vector<size_t> ids;
  for (size_t i = 0; i < splits.size(); i++) {
    if (isValidOptionName(splits[i])) {
      ids.push_back(i);
    }
  }
  return ids;
}

size_t getOptionSplitCount(const SimpleParamParser::Pattern& pattern) {
  size_t optionSplitCount = 0;
  for (auto& opt : pattern.options) {
    if (opt.type != ArgType::None) {
      optionSplitCount++;
    } else {
      optionSplitCount += 2;
    }
  }
  return optionSplitCount;
}

size_t getOptionArgTypeCount(const SimpleParamParser::Pattern& pattern) {
  size_t optionArgTypeCount = 0;
  for (auto& opt : pattern.options) {
    if (opt.type != ArgType::None) {
      optionArgTypeCount++;
    }
  }
  return optionArgTypeCount;
}

size_t getNonOptionalValueCount(const SimpleParamParser::Pattern& pattern) {
  size_t nonOptionalValueCount = 0;
  for (auto& val : pattern.values) {
    if (!val.optional) {
      nonOptionalValueCount++;
    }
  }
  return nonOptionalValueCount;
}

size_t getOptionalValueCount(const SimpleParamParser::Pattern& pattern) {
  size_t optionalValueCount = 0;
  for (auto& val : pattern.values) {
    if (val.optional) {
      optionalValueCount++;
    }
  }
  return optionalValueCount;
}

size_t getMultipleValueCount(const SimpleParamParser::Pattern& pattern) {
  size_t multipleValueCount = 0;
  for (auto& val : pattern.values) {
    if (val.multiple) {
      multipleValueCount++;
    }
  }
  return multipleValueCount;
}

std::optional<size_t> getMultipleValueID(
    const SimpleParamParser::Pattern& pattern) {
  for (size_t i = 0; i < pattern.values.size(); i++) {
    if (pattern.values[i].multiple) {
      return i;
    }
  }
  return {};
}

bool splitMatchesArgType(ArgType type, const std::string& str) {
  auto lower = Utilities::toLowerCase(str);
  switch (type) {
    case ArgType::Boolean:
      return str == "true" || str == "false";
    case ArgType::Integer:
      return Utilities::isInteger(str, true);
    case ArgType::PositiveInteger:
      return Utilities::isInteger(str, false);
    case ArgType::Real:
      return Utilities::isReal(str, true);
    case ArgType::PositiveReal:
      return Utilities::isReal(str, false);
    case ArgType::String:
      return true;
    default:
      return false;
  }
}

std::optional<std::set<size_t>> processOptions(
    const SimpleParamParser::Pattern& pattern,
    const std::vector<std::string>& splits, ParameterValues& vals) {
  std::set<size_t> processed;
  auto optionSplitIDs = getOptionSplitIds(splits);
  for (auto& id : optionSplitIDs) {
    auto currSplit = splits[id];
    if (isValidOptionName(currSplit)) {
      bool found = false;
      for (auto& opt : pattern.options) {
        if (opt.name == currSplit) {
          processed.insert(id);
          found = true;
          if (opt.type != ArgType::None) {
            if (id >= splits.size() - 1 ||
                !splitMatchesArgType(opt.type, splits[id + 1])) {
              return {};
            }
            vals[currSplit].push_back(splits[id + 1]);
            processed.insert(id + 1);
          } else {
            vals[currSplit] = {};
          }
        }
      }
      if (!found) return {};
    }
  }
  return processed;
}

std::vector<size_t> getActiveValues(size_t remainingSplits,
                                    const std::set<size_t>& processed,
                                    const SimpleParamParser::Pattern& pattern) {
  auto out = std::vector<size_t>();
  auto nonOptionalValueCount = getNonOptionalValueCount(pattern);
  size_t remainingOptionalValueCount = remainingSplits - nonOptionalValueCount;
  size_t optionalsActivated = 0;
  size_t valIndex = 0;
  for (size_t i = 0; i < remainingSplits && valIndex < pattern.values.size(); i++) {
    if (!processed.contains(valIndex)) {
      if (pattern.values[valIndex].optional &&
          optionalsActivated < remainingOptionalValueCount) {
        out.push_back(valIndex);
        valIndex++;
        optionalsActivated++;
      } else if (!pattern.values[valIndex].optional) {
        out.push_back(valIndex);
        valIndex++;
      }
      if (pattern.values[valIndex].multiple&&remainingOptionalValueCount != optionalsActivated){
        i += (remainingOptionalValueCount - optionalsActivated) - 1 ;
        optionalsActivated =
            remainingOptionalValueCount;  // disable optional counting after
                                          // multiple, should be a bool but im
                                          // too lazy
      }
    }
  }
  return out;
}

bool optionalAfterMultiple(const SimpleParamParser::Pattern& pattern) {
  bool hasMultipleValue = false;
  for (auto& val : pattern.values) {
    if (val.multiple) {
      hasMultipleValue = true;
    } else if (val.optional && hasMultipleValue) {
      return true;
    }
  }
  return false;
}

SimpleParamResult parseSplits(const SimpleParamParser::Pattern& pattern,
                              const std::vector<std::string>& splits) {
  ParameterValues vals;
  auto multipleValueCount = getMultipleValueCount(pattern),
       valueCount = getNonOptionalValueCount(pattern),
       optionalValueCount = getOptionalValueCount(pattern),
       optionSplitCount = getOptionSplitCount(pattern);

  if (multipleValueCount > 1 || optionalAfterMultiple(pattern))
    return SimpleParamResult::createResultShouldExit();

  if (splits.size() < valueCount)
    return SimpleParamResult::createResultShouldExit();
  if (multipleValueCount == 0 &&
      splits.size() > valueCount + optionalValueCount + optionSplitCount)
    return SimpleParamResult::createResultShouldExit();

  auto multipleID = getMultipleValueID(pattern);
  auto processedOpt = processOptions(pattern, splits, vals);
  if (!processedOpt) return SimpleParamResult::createResultShouldExit();
  auto processed = *processedOpt;

  auto activeVals =
      getActiveValues(splits.size() - processed.size(), processed, pattern);
  for (size_t j = 0; j < activeVals.size(); j++) {
    auto activeId = activeVals[j];
    auto& currVal = pattern.values[activeId];
    if (!multipleID || !currVal.multiple) {
      for (size_t i = 0; i < splits.size(); i++) {
        if (!processed.contains(i)) {
          processed.insert(i);
          vals[currVal.name].push_back(splits[i]);
          break;
        }
      }
    } else if (multipleID && currVal.multiple) {
      size_t remainingVals = activeVals.size() -processed.size();
      size_t remainingSplits = splits.size() - processed.size();
      size_t multipleValCount = remainingSplits - remainingVals;
      for (size_t k = 0; k < multipleValCount+1; k++) {
        for (size_t i = 0; i < splits.size(); i++) {
          if (!processed.contains(i)) {
            processed.insert(i);
            vals[currVal.name].push_back(splits[i]);
            break;
          }
        }
      }
    }
  }
  return SimpleParamResult::create(pattern.patternID, vals);
}

bool checkForHelp(const std::vector<std::string>& splits) {
  for (auto& split : splits) {
    if (split == "--help" || split == "-?") return true;
  }
  return false;
}

void SimpleParamParser::addPattern(const Pattern& pattern) {
  patterns.push_back(pattern);
}

SimpleParamResult SimpleParamParser::parse(int argc, char** argv) {
  std::stringstream stream;
  for (int i = 1; i < argc; i++) {
    stream << argv[i] << " ";
  }
  auto input = stream.str();
  return parse(input);
}

SimpleParamResult SimpleParamParser::parse(const std::string& arguments) {
  auto splits = Utilities::splitAtSpacesWithEscape(arguments);
  if (!checkForHelp(splits)) {
    for (auto& pattern : patterns) {
      auto ret = parseSplits(pattern, splits);
      if (!ret.shouldExit()) return ret;
    }
  }
  printHelp();
  return SimpleParamResult::createResultShouldExit();
}

SimpleParamParser::SimpleParamParser(std::ostream* cout) : console(cout) {}

void getUsageString(std::ostream& stream,
                    const SimpleParamParser::Pattern& pattern) {
  stream << Utilities::getExecutablePath().filename().generic_string();
  stream << " [options] ";
  for (auto& val : pattern.values) {
    if (val.optional) {
      stream << "[" << val.name << "] ";
    } else if (val.multiple) {
      stream << val.name << " ... ";
    } else {
      stream << val.name << " ";
    }
  }
}

void SimpleParamParser::printHelp() {
  if (console) {
    *console << "Usage :\n";
    bool first = true;
    for (auto& pattern : patterns) {
      if (first) {
        first = false;
        *console << "    ";
      } else {
        *console << "Or: ";
      }
      getUsageString(*console, pattern);
      *console << "\n";
    }

    size_t paramNameLenght = 0, optionNameLength = 0;
    std::map<std::string, std::string> valDescriptions, optDescriptions;
    for (auto& pattern : patterns) {
      for (auto& opt : pattern.options) {
        optDescriptions[opt.name] = pattern.descriptions[opt.name];
        if (opt.name.size() > optionNameLength) {
          optionNameLength = opt.name.size();
        }
      }
      for (auto& val : pattern.values) {
        valDescriptions[val.name] = pattern.descriptions[val.name];
        if (val.name.size() > paramNameLenght) {
          paramNameLenght = val.name.size();
        }
      }
    }
    if (!valDescriptions.empty()) {
      *console << "\nParameters:\n";
      for (auto& param : valDescriptions) {
        size_t padding = paramNameLenght - param.first.size();
        *console << "\t" << param.first << ": ";
        for (size_t i = 0; i < padding; i++) *console << " ";
        *console << param.second << "\n";
      }
    }

    if (!optDescriptions.empty()) {
      *console << "\nOptions:\n";
      for (auto& option : optDescriptions) {
        size_t padding = optionNameLength - option.first.size();
        *console << "\t" << option.first << ": ";
        for (size_t i = 0; i < padding; i++) *console << " ";
        *console << option.second << "\n";
      }
    }
  }
}
}  // namespace ConsoleUtils