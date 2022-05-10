#include "InputParser.h"
#include <Utilities/StringUtils.h>
namespace ConsoleUtils {
ParamResult InputParser::parse(std::shared_ptr<PatternToken> pattern,
                               const std::string& input) {
  auto splits = Utilities::splitAtSpacesWithEscape(input);
  // TODO implement this
  // Maybe list all possible permutations and match by length?
  // TODO implement validator
}

}  // namespace ConsoleUtils