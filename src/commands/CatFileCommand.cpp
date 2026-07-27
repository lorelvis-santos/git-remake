#include "commands/CatFileCommand.h"
#include "core/FileSystem.h"
#include "core/Compression.h"
#include <optional>
#include <vector>
#include <cstdint>
#include <iostream>
#include <algorithm>

CommandSpec CatFileCommand::get_command_spec() const {
  return {
    {},
    {},
    1,
    1
  };
}

int CatFileCommand::execute(const CommandArguments& args) {
  auto hash_operand = args.get_operand(0);

  if (!hash_operand) {
    return 1;
  }

  std::string hash(hash_operand.value());
  
  if (args.operands.empty() || args.operands[0].size() < 4) {
    return 1;
  }

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

  auto it = std::find(bytes.begin(), bytes.end(), '\0');

  if (it != bytes.end()) {
    // Para omitir el byte nulo
    auto payload_start = it + 1;
    size_t payload_size = std::distance(payload_start, bytes.end());
    std::cout.write(reinterpret_cast<const char*>(&*payload_start), payload_size);
    std::cout << '\n';
  }

  return 0;
}