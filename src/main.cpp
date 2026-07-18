#include "GitBlob.h"
#include <cstdio>

int main() {
  GitBlob blob(".ignore/test.txt");

  if (!blob.is_valid()) {
    printf("El blob no es válido\n");
    return 1;
  }

  printf("Tamaño del blob en bytes: %lu\n", blob.get_size());
  printf("Contenido del blob:\n");

  const char* data = blob.get_data();

  for (size_t i = 0; i < blob.get_size(); i++) {
    if (data[i] == '\0') {
      printf("\\0");
    } else {
      printf("%c", data[i]);
    }
  }

  printf("\n");

  return 0;
}