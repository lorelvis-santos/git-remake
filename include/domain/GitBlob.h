#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include <array>
#include <string>

class GitBlob {
  private:
  std::vector<uint8_t> bytes;

  public:
    GitBlob(std::vector<uint8_t> bytes);
    std::vector<uint8_t> serialize() &&;
};