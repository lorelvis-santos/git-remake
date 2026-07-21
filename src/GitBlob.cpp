#include "GitBlob.h"
#include "FileSystem.h"
#include "Crypto.h"
#include <iostream>
#include <zlib.h>
#include <sstream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <optional>

// int compress_blob(unsigned char** output, const char* source, uLong source_len) {
//   // Calculamos el máximo tamaño que puede ocupar el source_len de forma comprimida
//   uLong dest_len = compressBound(source_len);
  
//   // Alojamos la memoria en base a eso
//   *output = (unsigned char*)malloc(dest_len);

//   // Comprimimos
//   if (compress(*output, &dest_len, (const unsigned char*)source, source_len) != Z_OK) {
//     printf("Compression failed.\n");
//     return false;
//   }

//   printf("Compression successful!\n");
//   printf("Original size: %lu bytes -> Compressed size: %lu bytes\n", source_len, dest_len);

//   return 0;
// }

GitBlob::GitBlob(const std::string_view path) {
  // Aqui leemos el archivo directamente con nuestra utilidad de FileSystem...
  std::vector file = FileSystem::read_file(path);
  
  if (file.empty()) {
    std::cerr << "El archivo " << path << " no pudo ser abierto.\n";
    return;
  }
  
  std::string header = "blob " + std::to_string(file.size());
  
  this->data.assign(header.begin(), header.end());
  this->data.push_back('\0');
  this->data.insert(data.end(), file.begin(), file.end());

  std::optional<std::array<uint8_t, 32>> hash = Crypto::sha256(data.data(), data.size());

  if (!hash) {
    this->data.clear();
  }

  this->hash = hash.value();

  // Ahora toca comprimir el contenido
  // unsigned char* compressed_blob = (unsigned char*)malloc(0);
  // compress_blob(&compressed_blob, this->data, this->size);
}

std::string GitBlob::get_hash() const {
  std::stringstream ss;
  for (unsigned int i = 0; i < 32; ++i) {
    ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
  }
  return ss.str();
}