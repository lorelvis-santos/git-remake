#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <chrono>
#include <array>
#include <optional>

namespace Git {
  struct CommitSignature {
    std::string name;
    std::string email;
    std::chrono::system_clock::time_point timestamp;
    std::string timezone;
  };

  class Commit {
    public:
      Commit(
        std::array<uint8_t, 32> tree_hash,
        std::vector<std::array<uint8_t, 32>> parent_hashes,
        CommitSignature author,
        CommitSignature committer,
        std::string message
      );
      std::vector<uint8_t> serialize() &&;

    private:
      std::array<uint8_t, 32> tree_hash;
      std::vector<std::array<uint8_t, 32>> parents_hash;
      CommitSignature author;
      CommitSignature committer;
      std::string message;
  };
}