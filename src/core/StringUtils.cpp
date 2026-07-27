#include "core/StringUtils.h"
#include <string>

namespace StringUtils {
  bool starts_with(const std::string_view text, const std::string_view to_find) {
    return text.compare(0, to_find.size(), to_find) == 0;
  }

  bool contains(const std::string_view text, const std::string_view to_find) {
    return text.find(to_find) != std::string::npos;
  }

  std::vector<std::string_view> split(const std::string_view text, const std::string_view delimiter) {
    std::vector<std::string_view> tokens;
    
    if (delimiter.empty()) {
      if (!text.empty()) {
        tokens.push_back(text);
      }
      return tokens;
    }

    size_t pos = 0;
    size_t next_pos = 0;

    while ((next_pos = text.find(delimiter, pos)) != std::string_view::npos) {
      tokens.push_back(text.substr(pos, next_pos - pos));
      pos = next_pos + delimiter.length();
    }
    
    // Handle the remaining part of the string after the last delimiter
    tokens.push_back(text.substr(pos));
    return tokens;
  }

  std::string remove(const std::string_view text, const std::string_view to_remove) {
    if (to_remove.empty()) {
      return std::string(text);
    }

    std::string result;
    // Reservamos memoria para optimizar el rendimiento
    result.reserve(text.size()); 

    size_t pos = 0;
    size_t next_pos = 0;

    while ((next_pos = text.find(to_remove, pos)) != std::string_view::npos) {
      // Añade el fragmento de texto antes de la palabra a eliminar
      result.append(text.substr(pos, next_pos - pos));
      // Salta la palabra eliminada
      pos = next_pos + to_remove.length();
    }

    // Añade el fragmento final restante
    result.append(text.substr(pos));
    return result;
  }
}