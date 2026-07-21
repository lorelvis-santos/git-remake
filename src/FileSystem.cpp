#include "FileSystem.h"
#include <fstream>
#include <vector>
#include <iostream>

namespace FileSystem {
  // Función para leer los bytes de un archivo
  std::vector<uint8_t> read_file(const std::string_view path) {
    // std::ios::ate pone el cursor al final del archivo
    // la idea es contar los bytes que hay para reservar el espacio del vector
    // y luego poder leerlo todo

    // hay que pasar path como std::string(path) porque el string_view no 
    // asegura que el string termine con el byte nulo
    std::ifstream file(std::string(path), std::ios::binary | std::ios::ate);

    if (!file.is_open()) {
      return {};
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<uint8_t> buffer(size);

    if (file.read(reinterpret_cast<char*>(buffer.data()), size)) {
      std::cout << "Archivo leido correctamente. Bytes obtenidos: " << buffer.size() << "\n";
    }

    return buffer;
  }
}