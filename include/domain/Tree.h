#pragma once

#include <string>
#include <array>
#include <cstdint>
#include <vector>
#include <sstream>
#include <iomanip>

namespace Git {
  #define TREE_REGULAR_FILE_MODE "100644"
  #define TREE_EXECUTABLE_FILE_MODE "100755"
  #define TREE_DIR_MODE "40000"

  struct TreeEntry {
    std::string mode;
    std::string name;
    std::array<uint8_t, 32> hash; 
  };

  class Tree {
    public:
      Tree();
      void add_entry(const std::string& mode, const std::string& name, const std::array<uint8_t, 32>& hash);
      std::vector<uint8_t> serialize(); // esta funcion nace de la necesidad de llevar la fuente de la verdad al formato tangible de tree

    private:
      std::vector<TreeEntry> entries; // esta es la unica fuente de la verdad en GitTree, en GitBlob es la data del blob
      std::array<uint8_t, 32> hash;
  };
}