#include "domain/GitBlob.h"
#include "cli/CommandFactory.h"
#include "commands/HashObjectCommand.h"
#include "commands/CatFileCommand.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <string>

int main(int argc, char** argv) {
  if (argc < 3) {
    // Esto es solamente para el comando de hash-object, luego
    // lo adaptare mejor
    std::cout << "Usage: " << argv[0] << " <command> <path>\n";
    return 1;
  }

  // Registro del comando
  CommandFactory::register_command("hash-object", []() {
    return std::make_unique<HashObjectCommand>();
  });

  CommandFactory::register_command("cat-file", []() {
    return std::make_unique<CatFileCommand>();
  });

  auto command = CommandFactory::create(argv[1]);

  if (!command) {
    std::cerr << "Unknown command\n";
    return 1;
  }

  // sumamos uno para omitir el nombre del ejecutable y el comando
  std::vector<std::string_view> args(argv + 2, argv + argc);
  
  command->execute(args); // TODO: Manejar el retorno del execute

  // std::string_view path = "src/main.cpp";
  // GitBlob blob(path);

  // if (!blob.is_valid()) {
  //   std::cerr << "El blob no es válido" << std::endl;
  //   return 1;
  // }

  // std::cout << "Blob de " << path << ":\n" << std::string(blob.get_data().begin(), blob.get_data().end()) << std::endl;
  // std::cout << "Hash de " << path << ":\n" << blob.get_hash() << std::endl;

  return 0;
}