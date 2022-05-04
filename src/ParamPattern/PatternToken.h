#pragma once
#include <vector>
#include <memory>
#include "../ArgType.h"
namespace ConsoleUtils {

enum PatternTokenType {
  OPTION,    // -a /
  OPTIONAL,  // [a]
  MULTIPLE,  // a ...
  VALUE,     // a
  OR,        // a|b
  ORDER      // a b c
};

class PatternToken {
 protected:
  PatternToken(PatternTokenType type);

 public:
  virtual ~PatternToken() = default;
  PatternTokenType getType() const;

 private:
  const PatternTokenType type;
};

class NamedToken : public PatternToken {
 protected:
  NamedToken(PatternTokenType type, std::string name);

 public:
  ~NamedToken() = default;
  const std::string& getName() const;

 private:
  const std::string name;
};

class ParentToken : public PatternToken {
 protected:
  ParentToken(PatternTokenType type,
              std::vector<std::shared_ptr<PatternToken>> children);

 public:
  ~ParentToken() = default;
  std::vector<std::shared_ptr<PatternToken>> getChildren() const;

 private:
  std::vector<std::shared_ptr<PatternToken>> children;
};

class OptionToken : public NamedToken {
 public:
  OptionToken(std::string name, ArgType type = ArgType::None);
  ~OptionToken() = default;

  ArgType getArgType() const;

 private:
  const ArgType arg_type;
};

class OptionalToken : public ParentToken {
 public:
  OptionalToken(std::vector<std::shared_ptr<PatternToken>> children);
  ~OptionalToken() = default;
};

class MultipleToken : public NamedToken {
 public:
  MultipleToken(std::string name);
  ~MultipleToken() = default;
};

class ValueToken : public NamedToken {
 public:
  ValueToken(std::string name);
  ~ValueToken() = default;
};

class OrToken : public ParentToken {
 public:
  OrToken(std::vector<std::shared_ptr<PatternToken>> children);
  ~OrToken() = default;
};

class OrderToken : public ParentToken {
 public:
  OrderToken(std::vector<std::shared_ptr<PatternToken>> children);
  ~OrderToken() = default;
};

}  // namespace ConsoleUtils