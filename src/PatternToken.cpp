#include "PatternToken.h"
namespace ConsoleUtils {
PatternToken::PatternToken(PatternTokenType type) : type(type) {}

PatternTokenType PatternToken::getType() const { return type; }

ParentToken::ParentToken(PatternTokenType type,
                         std::vector<std::shared_ptr<PatternToken>> children)
    : PatternToken(type), children(children) {}

std::vector<std::shared_ptr<PatternToken>> ParentToken::getChildren() const {
  return children;
}
void ParentToken::addChild(std::shared_ptr<PatternToken> child) {
  children.push_back(child);
}

NamedToken::NamedToken(PatternTokenType type, std::string name)
    : PatternToken(type), name(name) {}

const std::string& NamedToken::getName() const { return name; }

OptionToken::OptionToken(std::string name, ArgType arg_type)
    : NamedToken(OPTION, name), arg_type(arg_type) {}

ArgType OptionToken::getArgType() const { return arg_type; }

OptionalToken::OptionalToken(
    std::vector<std::shared_ptr<PatternToken>> children)
    : ParentToken(OPTIONAL, children) {}

MultipleToken::MultipleToken(std::string name) : NamedToken(MULTIPLE, name) {}

ValueToken::ValueToken(std::string name) : NamedToken(VALUE, name) {}

OrToken::OrToken(std::vector<std::shared_ptr<PatternToken>> children)
    : ParentToken(OR, children) {}

OrderToken::OrderToken(std::vector<std::shared_ptr<PatternToken>> children)
    : ParentToken(ORDER, children) {}
}  // namespace ConsoleUtils