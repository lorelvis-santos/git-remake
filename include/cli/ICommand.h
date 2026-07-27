#pragma once

#include "cli/CommandArguments.h"
#include "cli/CommandSpec.h"
#include <vector>
#include <string>

class ICommand {
  public:
    virtual ~ICommand() = default;
    virtual int execute(const CommandArguments& args) = 0;
    virtual CommandSpec get_command_spec() const = 0;
};