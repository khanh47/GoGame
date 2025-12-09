RAYLIB_DIR ?= C:/msys64/ucrt64

CXX = g++
CXXFLAGS = -std=c++20 \
	-Iinclude \
	-Iinclude/Scene \
	-Iinclude/GameStates \
	-Iinclude/Menu \
	-Iinclude/Game \
	-Iinclude/AI \
	-Iinclude/GameData \
	-Iinclude/Resource \
	-Iinclude/Commands \
	-Iinclude/Render/InGame \
	-Iinclude/Render/Settings \
	-Iinclude/Menu/MenuComponent \
	-Iinclude/Menu/MenuView \
	-Iinclude/Menu/MenuController \
	-Iinclude/Scene/ConcreteScene \
	-Iinclude/GameStates/ConcreteGameStates \
	-I$(RAYLIB_DIR)/include -O2
LDFLAGS = -L$(RAYLIB_DIR)/lib -lraylib -lopengl32 -lgdi32 -lwinmm -luser32

SRC = $(wildcard src/*.cpp \
                 src/*/*.cpp \
                 src/*/*/*.cpp)
TARGET = run.exe

all: clean $(TARGET)
	@echo "Build complete! Running program..."
	@./$(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGET)