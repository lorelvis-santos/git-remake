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

GitBlob::GitBlob(std::vector<uint8_t> bytes) {
  this->bytes = std::move(bytes);
}

std::vector<uint8_t> GitBlob::serialize() && {
  std::vector<uint8_t> buffer;

  std::string header = "blob " + std::to_string(this->bytes.size());
  
  buffer.reserve(header.size() + this->bytes.size() + 1);
  buffer.assign(header.begin(), header.end());
  buffer.push_back('\0');
  buffer.insert(
    buffer.end(),
    std::make_move_iterator(bytes.begin()),
    std::make_move_iterator(bytes.end())
  );

  return buffer;
}