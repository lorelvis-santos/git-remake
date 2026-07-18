#pragma once

#include <cstddef>

class GitBlob {
  private:
    char* data;
    size_t size;
  public:
    GitBlob(const char* path);
    ~GitBlob();

    const char* get_data() const {
      return this->data;
    }

    size_t get_size() const {
      return size;
    }

    bool is_valid() const {
      return this->data != nullptr;
    }
};