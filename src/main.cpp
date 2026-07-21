#include "GitBlob.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>

int main() {
  std::string_view path = "src/main.cpp";
  GitBlob blob(path);

  if (!blob.is_valid()) {
    std::cerr << "El blob no es válido" << std::endl;
    return 1;
  }

  std::cout << "Blob de " << path << ":\n" << std::string(blob.get_data().begin(), blob.get_data().end()) << std::endl;
  std::cout << "Hash de " << path << ":\n" << blob.get_hash() << std::endl;

  // printf("Tamaño del blob en bytes: %lu\n", blob.get_size());
  // printf("Contenido del blob:\n");

  // const char* data = blob.get_data();

  // for (size_t i = 0; i < blob.get_size(); i++) {
  //   if (data[i] == '\0') {
  //     printf("\\0");
  //   } else {
  //     printf("%c", data[i]);
  //   }
  // }
  // printf("\n");

  // char* hash = (char*)malloc(65);
  // blob.get_hash(hash);
  // printf("hash: %s\n", hash);

  return 0;
}