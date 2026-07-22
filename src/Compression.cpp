#include "Compression.h"
#include <optional>
#include <cstdint>
#include <vector>

namespace Compression {
  std::optional<std::vector<uint8_t>> compress_blob(const unsigned char* source, uLong source_len) {
    uLong dest_len = compressBound(source_len);

    std::vector<uint8_t> output(dest_len);

    if (compress(output.data(), &dest_len, (const unsigned char*)source, source_len) != Z_OK) {
      std::cerr << "Compression failed.\n";
      return std::nullopt;
    }

    output.resize(dest_len);

    std::cout << "Compression succesful!\n";
    std::cout << "Original size: " << source_len << " bytes -> Compressed size: " << dest_len << " bytes\n";

    return output;
  }
}