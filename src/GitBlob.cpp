#include "GitBlob.h"
#include <cstdio>
#include <sys/stat.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <cstdint>
#include <zlib.h>

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

int compute_sha256(const char *string, unsigned char *output_hash, size_t size) {
  EVP_MD_CTX *context = EVP_MD_CTX_new();
  unsigned int internal_length;

  if (context == NULL) {
    return 1;
  }

  // Initialize the context with SHA-256 algorithm
  if (!EVP_DigestInit_ex(context, EVP_sha256(), NULL)) {
    EVP_MD_CTX_free(context);
    return 1;
  }

  // Pass the data to be hashed
  if (!EVP_DigestUpdate(context, string, size)) {
    EVP_MD_CTX_free(context);
    return 1;
  }

  // Finalize the hash computation
  if (!EVP_DigestFinal_ex(context, output_hash, &internal_length)) {
    EVP_MD_CTX_free(context);
    return 1;
  }

  // Clean up allocated context memory
  EVP_MD_CTX_free(context);
  return 0;
}

int compress_blob(unsigned char** output, const char* source, uLong source_len) {
  // Calculamos el máximo tamaño que puede ocupar el source_len de forma comprimida
  uLong dest_len = compressBound(source_len);
  
  // Alojamos la memoria en base a eso
  *output = (unsigned char*)malloc(dest_len);

  // Comprimimos
  if (compress(*output, &dest_len, (const unsigned char*)source, source_len) != Z_OK) {
    printf("Compression failed.\n");
    return 1;
  }

  printf("Compression successful!\n");
  printf("Original size: %lu bytes -> Compressed size: %lu bytes\n", source_len, dest_len);

  return 0;
}

// Fin de helpers =============================================

GitBlob::GitBlob(const char* path) {
  this->data = nullptr;
  this->size = 0;
  memset(this->hash, 0, sizeof(this->hash));

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

  int error = compute_sha256(this->data, this->hash, this->size);

  if (error == 1) {
    free(this->data);
    this->data = nullptr;
    this->size = 0;
  }

  // Ahora toca comprimir el contenido
  // unsigned char* compressed_blob = (unsigned char*)malloc(0);
  // compress_blob(&compressed_blob, this->data, this->size);
}

void GitBlob::get_hash(char* output) const {
  for (int i = 0; i < 32; i++) {
    snprintf(output + 2*i, 3, "%02x", this->hash[i]);
  }
}