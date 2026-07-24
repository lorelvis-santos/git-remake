#include "commands/CatFileCommand.h"
#include "core/FileSystem.h"
#include "core/Compression.h"
#include <optional>
#include <vector>
#include <cstdint>
#include <iostream>

// TODO: optimizar para que hayan busquedas en base a un hash de min 4 caracteres
int CatFileCommand::execute(const std::vector<std::string_view>& args) {
  std::string hash = "c0a56d50b43e3810bd0eb393da305b3202e4e457594d8d8d6acc51d70ec37580";

  std::optional<std::vector<uint8_t>> result = FileSystem::read_object(hash);

  if (!result) {
    std::cerr << "Hubo un error al leer el objeto" << std::endl;
    return 1;
  }

  std::optional<std::vector<uint8_t>> decompressed_data = Compression::decompress_blob(result.value());

  if (!decompressed_data) {
    std::cerr << "No se pudo decomprimir el objeto" << std::endl;
    return 1;
  }

  std::vector<uint8_t> bytes = decompressed_data.value();

  std::cout << bytes.size() << std::endl;
  std::cout << std::string(bytes.begin(), bytes.end()) << std::endl;

  return 0;
}