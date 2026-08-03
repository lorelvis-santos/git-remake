#include "domain/Tree.h"
#include "core/Crypto.h"
#include "core/Compression.h"
#include "core/FileSystem.h"
#include <algorithm>

Git::Tree::Tree() {
  this->entries = {};
}

// Metodos publicos

void Git::Tree::add_entry(const std::string& mode, const std::string& name, const std::array<uint8_t, 32>& hash) {
  this->entries.emplace_back(TreeEntry {
    mode,
    name,
    hash
  });
}

// Metodos privados

// Transforma los entries en un bloque contiguo en la memoria bajo el formato
// tree BYTES\0mode name\0size (y se repite)
std::vector<uint8_t> Git::Tree::serialize() {
  // Ordenar los entries por el nombre
  std::sort(this->entries.begin(), this->entries.end(), [](const TreeEntry& a, const TreeEntry& b) {
    return a.name < b.name;
  });

  size_t tree_size = 0;
  for (const auto& entry : this->entries) {
    tree_size += entry.mode.size() + 1 + entry.name.size() + 1 + 32;
  }

  // Preparar el bloque continuo de todo
  std::string header = "tree " + std::to_string(tree_size);
  std::vector<uint8_t> buffer;

  buffer.reserve(header.size() + tree_size + 1);
  buffer.assign(header.begin(), header.end());
  buffer.push_back('\0');
  
  for (const auto& entry : this->entries) {
    buffer.insert(
      buffer.end(),
      entry.mode.begin(),
      entry.mode.end()
    );
    buffer.push_back(' ');
    buffer.insert(
      buffer.end(),
      entry.name.begin(),
      entry.name.end()
    );
    buffer.push_back('\0');
    buffer.insert(
      buffer.end(),
      std::begin(entry.hash),
      std::end(entry.hash)
    );
  }

  return buffer;
}
