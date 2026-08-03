#include "commands/HashObjectCommand.h"
#include "domain/Blob.h"
#include "core/StringUtils.h"
#include "core/FileSystem.h"
#include "core/Crypto.h"
#include "core/Compression.h"
#include <iostream>
#include <algorithm>
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
  auto path_success = args.get_operand(0);

  if (!path_success || path_success.value().empty()) {
    std::cerr << "La ruta del archivo no es válida" << std::endl;
    return 1;
  }

  std::string_view path = path_success.value();
  std::optional<std::vector<uint8_t>> file = FileSystem::read_file(path);

  if (!file) {
    return 1;
  }

  Git::Blob blob(std::move(file.value()));
  auto buffer = std::move(blob).serialize();

  std::optional<std::array<uint8_t, 32>> hash_success = Crypto::sha256(buffer.data(), buffer.size());

  if (!hash_success) {
    std::cerr << "Hubo un error al hashear el contenido del blob" << std::endl;
    return 1;
  }

  auto hash_bytes = hash_success.value();

  bool save_blob = false;

  if (args.has_flag("w") || args.has_flag("write")) {
    save_blob = true;
  }

  std::string hash = Crypto::to_string(hash_bytes.data(), hash_bytes.size());
  
  if (save_blob) {
    // Ahora toca comprimir el contenido
    std::optional<std::vector<uint8_t>> compressed = Compression::compress_blob(buffer.data(), buffer.size());

    if (!compressed) {
      return 1;
    }

    if (!FileSystem::write_object(Crypto::to_string(hash_bytes.data(), hash_bytes.size()), compressed.value())) {
      return 1;
    }
  }

  std::cout << hash << std::endl;

  return 0;
}