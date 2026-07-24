#include "commands/HashObjectCommand.h"
#include "domain/GitBlob.h"
#include <iostream>
#include <vector>

int HashObjectCommand::execute(const std::vector<std::string_view>& args) {
  std::string_view path;
  bool save_blob = false;

  for (auto it = args.begin(); it != args.end(); ++it) {
    if (*it == "-w") {
      save_blob = true;
      continue;
    }

    // TODO: debo de mejorar la determinacion de path
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