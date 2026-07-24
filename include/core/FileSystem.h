#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <optional>
#include <filesystem>

namespace fs = std::filesystem;

namespace FileSystem {
  std::optional<std::vector<uint8_t>> read_file(const fs::path& path);
  std::optional<std::vector<uint8_t>> read_object(const std::string& hash);
  bool write_object(const std::string& hash, const std::vector<uint8_t>& bytes);
}