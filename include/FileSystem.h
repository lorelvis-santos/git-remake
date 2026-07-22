#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <optional>

namespace FileSystem {
  std::optional<std::vector<uint8_t>> read_file(const std::string_view path);
  bool write_object(const std::string& hash, const std::vector<uint8_t>& bytes);
}