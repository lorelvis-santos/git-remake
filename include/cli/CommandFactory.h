#pragma once

#include "ICommand.h"
#include <unordered_map>
#include <string>
#include <functional>
#include <memory>

class CommandFactory {
  public:
    using CreatorFunc = std::function<std::unique_ptr<ICommand>()>;

    // Metodos para registrar y crear
    static void register_command(const std::string& name, CreatorFunc creator);
    static std::unique_ptr<ICommand> create(const std::string& name);
  
  private:
    static std::unordered_map<std::string, CreatorFunc> registry;
};