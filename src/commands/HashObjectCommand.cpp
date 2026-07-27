#include "commands/HashObjectCommand.h"
#include "domain/GitBlob.h"
#include "core/StringUtils.h"
#include <iostream>
#include <vector>

namespace str = StringUtils;

CommandSpec HashObjectCommand::get_command_spec() const {
  return {
    {"w", "write"},
    {},
    1,
    1
  };
}

int HashObjectCommand::execute(const CommandArguments& args) {
  auto result = args.get_operand(0);

  if (!result) {
    return 1;
  }

  std::string_view path = result.value();

  bool save_blob = false;
  
  if (args.has_flag("w") || args.has_flag("write")) {
    save_blob = true;
  }

  if (path.empty()) {
    std::cerr << "La ruta del archivo no es válida" << std::endl;
    return 1;
  }

  GitBlob blob(path);

  if (!blob.is_valid()) {
    std::cerr << "El blob no es válido" << std::endl;
    return 1;
  }

  if (save_blob && !blob.save()) {
    return 1;
  }

  std::cout << blob.get_hash() << std::endl;

  return 0;
}