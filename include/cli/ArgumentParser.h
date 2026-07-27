#pragma once

#include <cli/CommandArguments.h>
#include <vector>
#include <string_view>

namespace ArgumentParser {
  CommandArguments parse(const std::vector<std::string_view>& raw_args);
}