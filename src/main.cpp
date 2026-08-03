#include "cli/CommandFactory.h"
#include "cli/ArgumentParser.h"
#include "cli/ArgumentValidation.h"
#include "commands/HashObjectCommand.h"
#include "commands/CatFileCommand.h"
#include "commands/WriteTreeCommand.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <string>

constexpr std::string_view to_string(ArgumentValidation::ValidationError error) {
  switch (error) {
    case ArgumentValidation::ValidationError::None:                return "None";
    case ArgumentValidation::ValidationError::MissingOption:       return "MissingOption";
    case ArgumentValidation::ValidationError::InvalidOption:       return "InvalidOption";
    case ArgumentValidation::ValidationError::InvalidFlag:         return "InvalidFlag";
    case ArgumentValidation::ValidationError::InvalidOperandCount: return "InvalidOperandCount";
    default: return "UnknownError";
  }
}

// Sobrecarga opcional para usar directamente con std::cout
std::ostream& operator<<(std::ostream& os, ArgumentValidation::ValidationError error) {
    return os << to_string(error);
}

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "Please provide at least one command\n";
    return 1;
  }

  // Registro del comando
  CommandFactory::register_command("hash-object", []() {
    return std::make_unique<HashObjectCommand>();
  });

  CommandFactory::register_command("cat-file", []() {
    return std::make_unique<CatFileCommand>();
  });

  CommandFactory::register_command("write-tree", []() {
    return std::make_unique<WriteTreeCommand>();
  });

  auto command = CommandFactory::create(argv[1]);

  if (!command) {
    std::cerr << "Unknown command\n";
    return 1;
  }

  // sumamos uno para omitir el nombre del ejecutable y el comando
  std::vector<std::string_view> raw_args(argv + 2, argv + argc);
  
  CommandArguments args = ArgumentParser::parse(raw_args);
  
  auto validation = ArgumentValidation::validate(command->get_command_spec(), args);

  if (!validation.is_valid) {
    std::cerr << "Hubo un error de validación al ejecutar el comando: " << argv[1] << "\n\n";
    std::cout << "Detalles del error:\n";
    std::cout << "\t- Tipo de error: " << validation.error << "\n";
    std::cout << "\t- Referencia del error: " << validation.argument_reference << "\n";
    std::cout << "\t- Mensaje del error: " << validation.message << "\n";
    
    return 1;
  }

  command->execute(args); // TODO: Manejar el retorno del execute

  return 0;
}