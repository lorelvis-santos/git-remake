#pragma once

#include <string_view>

namespace StringUtils {
  bool starts_with(const std::string_view text, const std::string_view to_find);
}