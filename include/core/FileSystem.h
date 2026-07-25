#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <optional>
#include <filesystem>

namespace fs = std::filesystem;

namespace FileSystem {
  std::optional<std::vector<uint8_t>> read_file(const fs::path& path);

  fs::path build_object_dir(const std::string& hash);
  fs::path build_object_path(const std::string& hash);
  
  std::string get_object_name(const std::string& hash);
  std::optional<fs::path> get_object_path(const std::string& hash);

  std::optional<std::vector<uint8_t>> read_object(const std::string& hash);
  bool write_object(const std::string& hash, const std::vector<uint8_t>& bytes);
}