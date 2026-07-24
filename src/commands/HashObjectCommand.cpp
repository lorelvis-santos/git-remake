#include "commands/HashObjectCommand.h"
#include "domain/GitBlob.h"
#include "core/StringUtils.h"
#include <iostream>
#include <vector>

namespace str = StringUtils;

int HashObjectCommand::execute(const std::vector<std::string_view>& args) {
  std::string_view path;
  bool save_blob = false;

  for (auto it = args.begin(); it != args.end(); ++it) {
    // TODO: Organizar esto mejor. El fin seria que lo de los argumentos sea mas organizado y automatizado.
    if (str::starts_with(*it, "-")) {
      if (*it == "-w" || *it == "--write") {
        save_blob = true;
        continue;
      }

      std::cerr << "Argumento inválido, solo permitido: -w o --write" << std::endl;
      return 1;
    }

    if (!path.empty()) {
      std::cerr << "Se ha proporcionado más de una ruta" << std::endl;
      return 1;
    }

    path = *it;
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