#pragma once

#include <unordered_set>
#include <unordered_map>
#include <string_view>
#include <vector>
#include <optional>

struct CommandArguments {
  std::unordered_set<std::string_view> flags;
  std::unordered_map<std::string_view, std::string_view> options;
  std::vector<std::string_view> operands;

  bool has_flag(std::string_view flag) const;
  std::optional<std::string_view> get_option(std::string_view option) const;
  std::optional<std::string_view> get_operand(size_t index) const;
};