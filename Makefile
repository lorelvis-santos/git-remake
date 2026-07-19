# Variables de configuracion
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude

# Rutas y archivos
SRC_DIR = src
BUILD_DIR = build
TARGET = git-rk

# Busqueda automatica de archivos
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Regla principal
all: dir $(TARGET)

# Ensamble final
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lcrypto

# Compilación de Archivos Intermedios (.o)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regla de Utilidad: Crear carpeta build
dir:
	mkdir -p $(BUILD_DIR)

# Regla de Limpieza
clean:
	rm -rf $(BUILD_DIR)/*.o $(TARGET)