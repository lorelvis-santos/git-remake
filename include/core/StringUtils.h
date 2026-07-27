#pragma once

#include <string_view>
#include <vector>

namespace StringUtils {
  bool starts_with(const std::string_view text, const std::string_view to_find);
  bool contains(const std::string_view text, const std::string_view to_find);
  std::vector<std::string_view> split(const std::string_view text, const std::string_view delimiter);
  std::string remove(const std::string_view text, const std::string_view to_remove);
}