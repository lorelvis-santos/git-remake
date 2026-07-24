#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include <array>
#include <string>

class GitBlob {
  private:
    std::vector<uint8_t> data;
    std::array<uint8_t, 32> hash;

  public:
    GitBlob(const std::string_view path);

    const std::vector<uint8_t>& get_data() const {
      return this->data;
    }
    
    std::string get_hash() const;

    bool is_valid() const {
      return !this->data.empty();
    }

    bool save() const;
};