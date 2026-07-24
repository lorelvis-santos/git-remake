#include "core/Compression.h"
#include <optional>
#include <cstdint>
#include <vector>
#include <cstring>

namespace Compression {
  std::optional<std::vector<uint8_t>> compress_blob(const unsigned char* source, uLong source_len) {
    uLong dest_len = compressBound(source_len);

    std::vector<uint8_t> output(dest_len);

    if (compress(output.data(), &dest_len, (const unsigned char*)source, source_len) != Z_OK) {
      // std::cerr << "Compression failed.\n";
      return std::nullopt;
    }

    output.resize(dest_len);

    // std::cout << "Compression succesful!\n";
    // std::cout << "Original size: " << source_len << " bytes -> Compressed size: " << dest_len << " bytes\n";

    return output;
  }

  std::optional<std::vector<uint8_t>> decompress_blob(const std::vector<uint8_t>& compressed_data) {
    if (compressed_data.empty()) {
      return std::nullopt;
    }

    // Inicializamos la estructura del stream de zlib
    z_stream stream;
    std::memset(&stream, 0, sizeof(stream));

    stream.next_in = const_cast<Bytef*>(compressed_data.data());
    stream.avail_in = static_cast<uInt>(compressed_data.size());

    // Inicializamos la descomprension
    if (inflateInit(&stream) != Z_OK) {
      return std::nullopt;
    }

    // TODO: extraer el tamaño de bytes de la cabecera del blob para
    // hacer un reserve en uncompressed, asi optimizamos la memoria
    std::vector<uint8_t> uncompressed;
    uint8_t buffer[4096];

    int ret;
    // Decompress chunk by chunk loop
    do {
      stream.next_out = reinterpret_cast<Bytef*>(buffer);
      stream.avail_out = sizeof(buffer);

      ret = inflate(&stream, Z_NO_FLUSH);
      
      if (ret == Z_NEED_DICT || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
          inflateEnd(&stream);
          return std::nullopt;
      }

      // Calculate how much data was generated in this step
      size_t have = sizeof(buffer) - stream.avail_out;
      uncompressed.insert(uncompressed.end(), buffer, buffer + have);

    } while (ret != Z_STREAM_END);

    // Clean up zlib resources
    inflateEnd(&stream);
    return uncompressed;
  }
}