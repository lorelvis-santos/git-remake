#include "core/Compression.h"
#include <optional>
#include <cstdint>
#include <vector>
#include <cstring>
#include <charconv>

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

    std::vector<uint8_t> uncompressed;
    uint8_t buffer[4096];

    int ret;

    auto decompress_chunk = [&](int flush_mode) -> int {
      stream.next_out = reinterpret_cast<Bytef*>(buffer);
      stream.avail_out = sizeof(buffer);
      
      int ret = inflate(&stream, flush_mode);
      
      if (ret == Z_NEED_DICT || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
        return -1;
      }

      return ret;
    };

    ret = decompress_chunk(Z_NO_FLUSH);
    
    if (ret == -1) {
      inflateEnd(&stream);
      return std::nullopt;
    }

    // Calculate how much data was generated in this step
    size_t have = sizeof(buffer) - stream.avail_out;

    // Alojamos la memoria necesaria para el vector
    uint8_t* null_byte_ptr = (uint8_t*)std::memchr(buffer, '\0', have);
    uint8_t* space_byte_ptr = (uint8_t*)std::memchr(buffer, ' ', have);

    if (null_byte_ptr != nullptr && space_byte_ptr != nullptr) {
      size_t null_index = null_byte_ptr - buffer;
      size_t space_index = space_byte_ptr - buffer;
      size_t memory_to_allocate;

      const char* start_ptr = reinterpret_cast<const char*>(buffer + space_index + 1);
      const char* end_ptr = reinterpret_cast<const char*>(buffer + null_index);

      std::from_chars_result result = std::from_chars(start_ptr, end_ptr, memory_to_allocate);

      if (result.ec == std::errc()) {
        uncompressed.reserve(memory_to_allocate + null_index + 1);
      }
    }

    uncompressed.insert(uncompressed.end(), buffer, buffer + have);

    // Decompress chunk by chunk loop
    while (ret != Z_STREAM_END) {
      ret = decompress_chunk(Z_NO_FLUSH);
      
      if (ret == -1) {
        inflateEnd(&stream);
        return std::nullopt;
      }

      // Calculate how much data was generated in this step
      size_t have = sizeof(buffer) - stream.avail_out;
      uncompressed.insert(uncompressed.end(), buffer, buffer + have);

    }

    // Clean up zlib resources
    inflateEnd(&stream);
    return uncompressed;
  }
}