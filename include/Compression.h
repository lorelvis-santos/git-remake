#pragma once

#include <zlib.h>
#include <cstdint>
#include <vector>
#include <iostream>
#include <optional>

namespace Compression {
  std::optional<std::vector<uint8_t>> compress_blob(const unsigned char* source, uLong source_len);
}