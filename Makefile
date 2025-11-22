# ******************************************************************************
# Makefile - Digital Exodus Build Configuration
#
# Build targets:
#   make          - Build the game (without SFML audio)
#   make sfml     - Build with SFML audio support
#   make clean    - Remove build files
#   make run      - Build and run the game
#
# Author: Digital Exodus Project
# Course: COS30008 Data Structures and Patterns
# ******************************************************************************

# Compiler settings
# SFML 3.0 requires C++17 or later
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Output executable
TARGET = DigitalExodus

# Default build (without SFML)
all: $(BUILD_DIR) $(TARGET)
	@echo "Build complete: $(TARGET)"
	@echo "Run with: ./$(TARGET)"

# Build with SFML audio support
sfml: CXXFLAGS += -DSFML_AVAILABLE
sfml: LDFLAGS += -lsfml-audio -lsfml-system
sfml: all

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Link
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

# Clean build files
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET)
	rm -f *.dat
	rm -f saves/*.dat
	@echo "Clean complete"

# Build and run
run: all
	./$(TARGET)

# Run with valgrind for memory checking
memcheck: all
	valgrind --leak-check=full ./$(TARGET)

# Dependencies (simplified - recompile all if any header changes)
$(OBJECTS): $(wildcard $(INC_DIR)/*.h)

# Phony targets
.PHONY: all sfml clean run memcheck

# ******************************************************************************
# Visual Studio Project Notes (for Windows):
#
# To build in Visual Studio:
# 1. Create a new Empty C++ Console Application
# 2. Add all .h files from include/ to Header Files
# 3. Add all .cpp files from src/ to Source Files
# 4. Set C++ Language Standard to C++17 or later (required for SFML 3.0)
# 5. For SFML 3.0 audio:
#    a. Download SFML 3.0 from https://www.sfml-dev.org/
#    b. Add SFML include path to project includes
#    c. Add SFML lib path to library directories
#    d. Add sfml-audio.lib and sfml-system.lib to linker input
#    e. Define SFML_AVAILABLE in preprocessor definitions
#    f. Copy SFML DLLs to output directory
#
# ******************************************************************************
