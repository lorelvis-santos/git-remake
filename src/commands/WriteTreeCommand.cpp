#include "commands/WriteTreeCommand.h"
#include "domain/GitTree.h"
#include "domain/GitBlob.h"
#include "core/StringUtils.h"
#include "core/Crypto.h"
#include "core/Compression.h"
#include "core/FileSystem.h"
#include <iostream>
#include <vector>

namespace str = StringUtils;

CommandSpec WriteTreeCommand::get_command_spec() const {
  return {
    {},
    {},
    0,
    1
  };
}

int WriteTreeCommand::execute(const CommandArguments& args) {
  GitTree tree;

  // TODO: Llevar esto a funcionar con recursividad de abajo hacia arriba
  // manual, sin recursividad
  GitBlob blob_1("./test_tree/file.txt");
  GitBlob blob_2("./test_tree/file_2.txt");

  tree.add_entry(TREE_REGULAR_FILE_MODE, "file.txt", blob_1.get_hash_bytes());
  tree.add_entry(TREE_REGULAR_FILE_MODE, "file_2.txt", blob_2.get_hash_bytes());

  // obtenemos el buffer del tree
  std::vector<uint8_t> buffer = tree.serialize();

  // hasheamos el buffer
  auto hash_result = Crypto::sha256(buffer.data(), buffer.size());

  if (!hash_result) {
    return 1;
  }

  std::array<uint8_t, 32> hash_bytes = hash_result.value();

  // comprimimos
  auto compressed = Compression::compress_blob(buffer.data(), buffer.size());

  if (!compressed) {
    return 1;
  }

  std::string hash = Crypto::to_string(hash_bytes.data(), 32);

  // guardamos
  if (!FileSystem::write_object(hash, compressed.value())) {
    return 1;
  }

  std::cout << hash << std::endl;

  return 0;
}