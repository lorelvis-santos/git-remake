#pragma once

#include <cstddef>
#include <cstdint>

class GitBlob {
  private:
    char* data;
    size_t size;
    uint8_t hash[32];

  public:
    GitBlob(const char* path);
    ~GitBlob();


    const char* get_data() const {
      return this->data;
    }

    size_t get_size() const {
      return this->size;
    }
    
    void get_hash(char* output) const;

    bool is_valid() const {
      return this->data != nullptr;
    }

};