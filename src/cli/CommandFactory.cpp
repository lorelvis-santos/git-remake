#include "cli/CommandFactory.h"

std::unordered_map<std::string, CommandFactory::CreatorFunc> CommandFactory::registry;

void CommandFactory::register_command(const std::string& name, CreatorFunc creator) {
  registry[name] = creator;
}

std::unique_ptr<ICommand> CommandFactory::create(const std::string& name) {
  auto it = registry.find(name);

  if (it != registry.end()) {
    return it->second();
  }

  return nullptr;
}