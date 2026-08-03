#include "commands/WriteTreeCommand.h"
#include "domain/GitTree.h"
#include "domain/GitBlob.h"
#include "core/StringUtils.h"
#include "core/Crypto.h"
#include "core/Compression.h"
#include "core/FileSystem.h"
#include <iostream>
#include <vector>
#include <filesystem>

namespace str = StringUtils;
namespace fs = std::filesystem;

bool is_executable(const fs::directory_entry& entry) {
  fs::perms p = entry.status().permissions();
  return !entry.is_directory() && (p & (fs::perms::owner_exec | fs::perms::group_exec | fs::perms::others_exec)) != fs::perms::none;
}

std::optional<std::array<uint8_t, 32>> build_blob(fs::path target_file_path) {
  std::optional<std::vector<uint8_t>> file = FileSystem::read_file(target_file_path);

  if (!file) {
    return std::nullopt;
  }

  GitBlob blob(std::move(file.value()));

  auto buffer = std::move(blob).serialize();

  std::optional<std::array<uint8_t, 32>> hash_success = Crypto::sha256(buffer.data(), buffer.size());

  if (!hash_success) {
    return std::nullopt;
  }

  auto hash_bytes = hash_success.value();
  std::string hash = Crypto::to_string(hash_bytes.data(), hash_bytes.size());

  // Ahora toca comprimir el contenido
  std::optional<std::vector<uint8_t>> compressed = Compression::compress_blob(buffer.data(), buffer.size());

  if (!compressed) {
    return std::nullopt;
  }

  if (!FileSystem::write_object(Crypto::to_string(hash_bytes.data(), hash_bytes.size()), compressed.value())) {
    return std::nullopt;
  }

  return hash_bytes;
}

std::optional<std::array<uint8_t, 32>> build_tree(GitTree& tree) {
  // obtenemos el buffer del tree
  std::vector<uint8_t> buffer = tree.serialize();

  // hasheamos el buffer
  auto hash_result = Crypto::sha256(buffer.data(), buffer.size());

  if (!hash_result) {
    return std::nullopt;
  }

  std::array<uint8_t, 32> hash_bytes = hash_result.value();

  // comprimimos
  auto compressed = Compression::compress_blob(buffer.data(), buffer.size());

  if (!compressed) {
    return std::nullopt;
  }

  std::string hash = Crypto::to_string(hash_bytes.data(), 32);

  // guardamos
  if (!FileSystem::write_object(hash, compressed.value())) {
    return std::nullopt;
  }

  return hash_bytes;
}

std::optional<std::array<uint8_t, 32>> build_tree_recursive(fs::path target_dir_path) {
  GitTree tree;

  for (const auto& entry : fs::directory_iterator(target_dir_path)) {
    if ((entry.is_directory() &&
      (
        entry.path().filename() == ".git" || 
        entry.path().filename() == ".gitrk"
      )
    )) {
      continue;
    }

    if (entry.is_directory()) {
      auto hash = build_tree_recursive(entry.path());

      if (!hash) {
        return std::nullopt;
      }

      tree.add_entry(TREE_DIR_MODE, entry.path().filename(), hash.value());
      continue;
    }

    auto entry_path = entry.path();
    auto entry_filename = entry_path.filename();

    auto blob_creation_result = build_blob(entry_path);

    if (!blob_creation_result) {
      return std::nullopt;
    }

    auto blob_hash_bytes = blob_creation_result.value();

    if (is_executable(entry)) {
      tree.add_entry(TREE_EXECUTABLE_FILE_MODE, entry_filename, blob_hash_bytes);
      continue;
    }

    // si no es dir, ni es ejecutable, es regular
    tree.add_entry(TREE_REGULAR_FILE_MODE, entry_filename, blob_hash_bytes);
  }

  auto result = build_tree(tree);

  if (!result) {
    return std::nullopt;
  }
  
  return result.value();
}

CommandSpec WriteTreeCommand::get_command_spec() const {
  return {
    {},
    {},
    0,
    0
  };
}

int WriteTreeCommand::execute(const CommandArguments& args) {
  auto result = build_tree_recursive(".");

  if (!result) {
    return 1;
  }

  auto hash_bytes = result.value();

  std::cout << Crypto::to_string(hash_bytes.data(), hash_bytes.size()) << std::endl;

  return 0;
}