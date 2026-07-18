#include "GitBlob.h"
#include <cstdio>
#include <sys/stat.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>

// Helpers que despues seran refactorizados ===================

off64_t get_bytes_count_from_file(const char* path) {
  struct stat metadata;

  if (stat(path, &metadata) != 0) {
    return -1;
  }

  return metadata.st_size;
}

int write_bytes_into_buffer_from_file(char* buffer, uint64_t size, const char* path) {
  FILE *file_ptr = fopen(path, "rb");

  if (file_ptr == NULL) {
    return 1;
  }

  fread(buffer, size, 1, file_ptr);

  fclose(file_ptr);

  return 0;
}

// Fin de helpers =============================================

GitBlob::GitBlob(const char* path) {
  this->data = nullptr;
  this->size = 0;

  char header[32];
  off64_t file_size = get_bytes_count_from_file(path);

  if (file_size == -1) {
    printf("File doesn't exist\n");
    return;
  }

  size_t bytes_written = snprintf(header, sizeof(header), "blob %" PRIu64, (uint64_t)file_size);
  this->size = bytes_written + file_size + 1; // Agregamos un byte de mas por el byte nulo '\0'

  this->data = (char*) malloc(this->size);

  if (this->data == nullptr) {
    printf("Memory not allocated\n");
    return;
  }

  // Aplicamos la logica de memcpy.cpp
  memcpy(this->data, header, bytes_written + 1);

  // Ahora leemos el archivo en cuestion
  write_bytes_into_buffer_from_file(this->data + bytes_written + 1, file_size, path);
}

GitBlob::~GitBlob() {
  free(this->data);
  this->size = 0;
}