#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include <array>
#include <string>

namespace Git {
  class Blob {
    private:
    std::vector<uint8_t> bytes;

    public:
      Blob(std::vector<uint8_t> bytes);
      std::vector<uint8_t> serialize() &&;
  };
}