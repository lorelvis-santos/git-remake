#include "core/FileSystem.h"
#include "core/StringUtils.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <cstdint>

#define OBJECTS_PATH ".gitrk/objects"

namespace fs = std::filesystem;
namespace str = StringUtils;

namespace FileSystem {
  fs::path build_object_dir(const std::string& hash) {
    fs::path objects_dir(OBJECTS_PATH);
    std::string hash_start = hash.substr(0, 2);
    return fs::path(objects_dir / hash_start);
  }

  fs::path build_object_path(const std::string& hash) {
    fs::path object_dir = build_object_dir(hash);
    std::string hash_rest = hash.substr(2);
    return fs::path(object_dir / hash_rest);
  }

  std::string get_object_name(const std::string& hash) {
    return hash.substr(2);
  }

  // Función para leer los bytes de un archivo
  std::optional<std::vector<uint8_t>> read_file(const fs::path& path) {
    // std::ios::ate pone el cursor al final del archivo
    // la idea es contar los bytes que hay para reservar el espacio del vector
    // y luego poder leerlo todo

    // hay que pasar path como std::string(path) porque el string_view no 
    // asegura que el string termine con el byte nulo
    std::ifstream file(path, std::ios::binary | std::ios::ate);

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

  std::optional<fs::path> get_object_path(const std::string& hash) {
    // Verificamos que el hash sea minimo de 4 caracteres
    if (hash.size() < 4) {
      return std::nullopt;
    }
  
    fs::path object_dir = build_object_dir(hash);

    // Verificamos que el dir exista
    if (!fs::exists(object_dir) || !fs::is_directory(object_dir)) {
      return std::nullopt;
    }

    // Buscamos dentro del dir con el resto de la cadena del hash
    bool found = false;
    fs::path object_name;

    for (const auto& entry : fs::directory_iterator(object_dir)) {
      if (!entry.is_regular_file() || !str::starts_with(entry.path().filename().c_str(), get_object_name(hash))) {
        continue;
      }

      if (found) {
        return std::nullopt;
      }

      object_name = entry.path().filename();
      found = true;
    }

    if (!found) {
      return std::nullopt;
    }

    return fs::path(object_dir / object_name);
  }

  std::optional<std::vector<uint8_t>> read_object(const std::string& hash) {
    if (hash.size() < 4) {
      return std::nullopt;
    }

    std::optional<fs::path> object_path = get_object_path(hash);

    if (!object_path) {
      return std::nullopt;
    }

    return read_file(object_path.value());
  }

  // Función para persistir un blob/object
  bool write_object(const std::string& hash, const std::vector<uint8_t>& bytes) {
    if (hash.size() < 4) {
      return false;
    }

    fs::path object_dir = build_object_dir(hash);
    fs::path object_path = build_object_path(hash);

    // comprobar que la ruta del hash exista
    // si no, se crea
    if (!fs::exists(object_dir)) {
      fs::create_directories(object_dir);
    }

    // escribir el archivo con el resto del hash y con el contenido de bytes
    std::ofstream file(object_path , std::ios::out | std::ios::binary);

    if (!file.is_open()) {
      std::cerr << "No se pudo crear el archivo en la ruta " << object_path ;
      return false;
    }

    file.write(reinterpret_cast<const char*>(bytes.data()), bytes.size() * sizeof(uint8_t));

    return true;
  }
}