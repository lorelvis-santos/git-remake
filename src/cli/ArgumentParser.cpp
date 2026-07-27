#include <cli/ArgumentParser.h>
#include <iostream>

namespace ArgumentParser {
  CommandArguments parse(const std::vector<std::string_view>& raw_args) {
    CommandArguments parsed_args = {};

    for (auto arg : raw_args) {
      std::cout << arg << "\n";
    }

    return parsed_args;
  }
}