#include "cli/ArgumentValidation.h"

namespace ArgumentValidation {
  ValidationResult validate(const CommandSpec& commandSpec, const CommandArguments& commandArgs) {
    // Validamos primero los operandos posicionales
    size_t operands_count = commandArgs.operands.size();
    if (operands_count < commandSpec.min_operands_count) {
      return ValidationResult {
        false,
        ValidationError::InvalidOperandCount,
        "operand_count",
        "operand_count is lower than expected"
      };
    }

    if (operands_count > commandSpec.max_operands_count) {
      return ValidationResult {
        false,
        ValidationError::InvalidOperandCount,
        "operand_count",
        "operand_count is greater than expected"
      };
    }

    // Validamos los flags
    for (auto flag : commandArgs.flags) {
      if (commandSpec.flags.find(flag) == commandSpec.flags.end()) {
        return ValidationResult {
          false,
          ValidationError::InvalidFlag,
          flag,
          "invalid flag provided"
        };
      }
    }

    // Validamos las opciones
    for (auto option : commandSpec.options) {
      if (!option.second) {
        continue;
      }

      if (commandArgs.options.find(option.first) == commandArgs.options.end()) {
        return ValidationResult {
          false,
          ValidationError::MissingOption,
          option.first,
          "expected option is missing"
        };
      }
    }

    for (auto option : commandArgs.options) {
      if (commandSpec.options.find(option.first) == commandSpec.options.end()) {
        return ValidationResult {
          false,
          ValidationError::InvalidOption,
          option.first,
          "invalid option provided"
        };
      }
    }

    return ValidationResult {
      true,
      ValidationError::None
    };
  }
}