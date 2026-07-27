#pragma once

#include "cli/ICommand.h"

class HashObjectCommand : public ICommand {
  public:
    int execute(const CommandArguments& args) override;
    CommandSpec get_command_spec() const override;
};

