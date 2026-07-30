#pragma once

#include <string>
#include <array>
#include <cstdint>
#include <vector>

struct TreeEntry {
  std::string code;
  std::string name;
  std::array<uint8_t, 32> hash; 
};

class GitTree {
  public:
    GitTree();
    void add_entry(const std::string& mode, const std::string& name, const std::array<uint8_t, 32>& hash);
    std::array<uint8_t, 32> compute_hash();
  private:
    std::vector<TreeEntry> rows;
    bool open = true;
};