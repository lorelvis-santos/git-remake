#pragma once

#include <optional>
#include <array>
#include <cstdint>

namespace Crypto {
  std::optional<std::array<uint8_t, 32>> sha256(const uint8_t* data, size_t size);
}