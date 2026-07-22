#include "Crypto.h"
#include <openssl/evp.h>
#include <memory>

namespace Crypto {
  std::optional<std::array<uint8_t, 32>> sha256(const uint8_t* data, size_t size) {
    // Create and initialize the context
    std::unique_ptr<EVP_MD_CTX, decltype(&EVP_MD_CTX_free)> context(EVP_MD_CTX_new(), EVP_MD_CTX_free);
    if (!context) {
      return std::nullopt;
    }

    // Initialize the SHA-256 operation
    if (EVP_DigestInit_ex(context.get(), EVP_sha256(), nullptr) != 1) {
      return std::nullopt;
    }

    // Provide the message to be hashed
    if (EVP_DigestUpdate(context.get(), data, size) != 1) {
      return std::nullopt;
    }

    // Finalize the hash and fetch digest bytes
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int length = 0;

    if (EVP_DigestFinal_ex(context.get(), hash, &length) != 1) {
      return std::nullopt;
    }

    std::array<uint8_t, 32> bytes;
    std::copy(std::begin(hash), hash + length, bytes.begin());
    
    return bytes;
  }
}