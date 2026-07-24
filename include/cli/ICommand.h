#pragma once

#include <vector>
#include <string>

class ICommand {
  public:
    virtual ~ICommand() = default;
    virtual int execute(const std::vector<std::string_view>& args) = 0;
};