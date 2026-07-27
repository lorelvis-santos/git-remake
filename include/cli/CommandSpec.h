#pragma once

#include <unordered_set>
#include <unordered_map>
#include <string_view>
#include <vector>
#include <optional>

struct CommandSpec {
  std::unordered_set<std::string_view> flags;
  std::unordered_map<std::string_view, bool> options;
  size_t min_operands_count;
  size_t max_operands_count;
};