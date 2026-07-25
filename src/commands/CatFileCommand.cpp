#include "commands/CatFileCommand.h"
#include "core/FileSystem.h"
#include "core/Compression.h"
#include <optional>
#include <vector>
#include <cstdint>
#include <iostream>
#include <algorithm>

int CatFileCommand::execute(const std::vector<std::string_view>& args) {
  if (args.empty() || args[0].size() < 4) {
    return 1;
  }

  std::string hash(args[0]);

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