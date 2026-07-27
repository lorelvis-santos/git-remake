#pragma once

#include "cli/CommandSpec.h"
#include "cli/CommandArguments.h"
#include <string>

namespace ArgumentValidation {
  enum class ValidationError {
    None,
    MissingOption,
    InvalidOption,
    InvalidFlag,
    InvalidOperandCount
  };

  struct ValidationResult {
    bool is_valid;
    ValidationError error;
    std::string_view argument_reference;
    std::string message;
  };

  ValidationResult validate(const CommandSpec& commandSpec, const CommandArguments& commandArgs);
}