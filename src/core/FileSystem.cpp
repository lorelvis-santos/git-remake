#include "core/FileSystem.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <cstdint>

namespace fs = std::filesystem;

namespace FileSystem {
  // Función para leer los bytes de un archivo
  std::optional<std::vector<uint8_t>> read_file(const std::string_view path) {
    // std::ios::ate pone el cursor al final del archivo
    // la idea es contar los bytes que hay para reservar el espacio del vector
    // y luego poder leerlo todo

    // hay que pasar path como std::string(path) porque el string_view no 
    // asegura que el string termine con el byte nulo
    std::ifstream file(std::string(path), std::ios::binary | std::ios::ate);

    if (!file.is_open()) {
      return std::nullopt;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<uint8_t> buffer(size);

    if (file.read(reinterpret_cast<char*>(buffer.data()), size)) {
      // std::cout << "Archivo leido correctamente. Bytes obtenidos: " << buffer.size() << "\n";
    }

    return buffer;
  }

  // Función para persistir un blob/object
  bool write_object(const std::string& hash, const std::vector<uint8_t>& bytes) {
    // Pasos para escribir el object
    
    // path base, esto hay que refactorizarlo
    fs::path dir(".gitrk/objects");

    // separar las primeras dos letras del resto del hash.
    std::string hash_start = hash.substr(0, 2);
    std::string file_name = hash.substr(2);

    // construir paths del object
    fs::path dir_path(dir / hash_start);
    fs::path file_path(dir_path / file_name);

    // comprobar que la ruta del hash exista
    // si no, se crea
    if (!fs::exists(dir_path)) {
      fs::create_directories(dir_path);
    }

    // escribir el archivo con el resto del hash y con el contenido de bytes
    std::ofstream file(file_path, std::ios::out | std::ios::binary);

    if (!file.is_open()) {
      std::cerr << "No se pudo crear el archivo en la ruta " << file_path;
      return false;
    }

    file.write(reinterpret_cast<const char*>(bytes.data()), bytes.size() * sizeof(uint8_t));

    return true;
  }
}