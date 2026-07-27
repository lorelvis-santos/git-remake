#include <cli/ArgumentParser.h>
#include <core/StringUtils.h>
#include <iostream>

namespace str = StringUtils;

namespace ArgumentParser {
  CommandArguments parse(const std::vector<std::string_view>& raw_args) {
    CommandArguments parsed_args = {};
    
    for (auto arg : raw_args) {
      if (!str::starts_with(arg, "-")) {
        parsed_args.operands.push_back(arg);
        continue;
      }

      std::string_view token = arg;

      if (token.size() >= 2 && token.substr(0, 2) == "--") {
        token.remove_prefix(2);
      } else if (token.size() >= 1 && token[0] == '-') {
        token.remove_prefix(1);
      }

      auto separator_index = token.find("=");
      if (separator_index != std::string::npos) {
        parsed_args.options[token.substr(0, separator_index)] = token.substr(separator_index + 1);
        continue;
      }

      // parametro por defecto -p, --parametro
      parsed_args.flags.insert(token);
    }

    return parsed_args;
  }
}