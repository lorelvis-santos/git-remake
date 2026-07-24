#include "domain/GitBlob.h"
#include "core/FileSystem.h"
#include "core/Crypto.h"
#include "core/Compression.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <optional>

GitBlob::GitBlob(const std::string_view path) {
  // Aqui leemos el archivo directamente con nuestra utilidad de FileSystem...
  std::optional<std::vector<uint8_t>> result = FileSystem::read_file(path);

  if (!result) {
    return;
  }
  
  std::vector<uint8_t> file = std::move(result.value());
  
  std::string header = "blob " + std::to_string(file.size());
  
  this->data.reserve(header.size() + file.size() + 1);
  this->data.assign(header.begin(), header.end());
  this->data.push_back('\0');
  this->data.insert(
    this->data.end(),
    std::make_move_iterator(file.begin()),
    std::make_move_iterator(file.end())
  );

  std::optional<std::array<uint8_t, 32>> hash = Crypto::sha256(data.data(), data.size());

  if (!hash) {
    this->data.clear();
    return;
  }

  this->hash = hash.value();
}

bool GitBlob::save() const { 
  // Ahora toca comprimir el contenido
  std::optional<std::vector<uint8_t>> compressed = Compression::compress_blob(this->data.data(), this->data.size());

  if (!compressed) {
    return false;
  }

  // std::cout << "Se comprimió el archivo de " << this->data.size() << " bytes a " << compressed.value().size() << " bytes\n";

  if (!FileSystem::write_object(this->get_hash(), compressed.value())) {
    return false;
  }

  return true;
}

std::string GitBlob::get_hash() const {
  std::stringstream ss;
  for (unsigned int i = 0; i < 32; ++i) {
    ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
  }
  return ss.str();
}