#include "cli/CommandArguments.h"

bool CommandArguments::has_flag(std::string_view flag) const {
  if (this->flags.find(flag) == this->flags.end()) {
    return false;
  }
  return true;
}

std::optional<std::string_view> CommandArguments::get_option(std::string_view option) const {
  auto it = this->options.find(option);
  if (it == this->options.end()) {
    return std::nullopt;
  }
  return it->second;
}

std::optional<std::string_view> CommandArguments::get_operand(size_t index) const {
  if (index >= this->operands.size()) {
    return std::nullopt;
  }
  return this->operands[index];
}

