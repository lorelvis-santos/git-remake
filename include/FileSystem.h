#pragma once

#include <vector>
#include <cstdint>
#include <string>

namespace FileSystem {
  std::vector<uint8_t> read_file(const std::string_view path);
}