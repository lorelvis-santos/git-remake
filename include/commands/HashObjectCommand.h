#pragma once

#include "cli/ICommand.h"

class HashObjectCommand : public ICommand {
  public:
    int execute(const std::vector<std::string_view>& args) override;
};

