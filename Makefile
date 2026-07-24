# Variables de configuracion
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude
LDFLAGS = -lcrypto -lz

# Rutas y archivos
SRC_DIR = src
BUILD_DIR = build

# Búsqueda recursiva de archivos fuente
SOURCES = $(shell find $(SRC_DIR) -name '*.cpp')

# Mapeo de rutas de src/*.cpp a build/*.o
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Regla principal
TARGET = gitrk
all: $(TARGET)

# Ensamble final
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Creación dinámica de directorios y compilación de objetos
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regla de Limpieza
clean:
	rm -rf $(BUILD_DIR)/*.o $(TARGET)