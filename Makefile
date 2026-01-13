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

# Web build configuration
WEB_TARGET = gogame
EMCC = emcc
EMCC_FLAGS = -std=c++20 \
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
	-O3 \
	-s USE_GLFW=3 \
	-s ASYNCIFY \
	-s TOTAL_MEMORY=67108864 \
	-s ALLOW_MEMORY_GROWTH=1 \
	--preload-file assets \
	--shell-file web/shell.html

.PHONY: all clean web clean-web help

all: clean $(TARGET)
	@echo "Build complete! Running program..."
	@./$(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Web build target
web: $(SRC)
	@echo "Building for web..."
	@mkdir -p web_build
	$(EMCC) $(EMCC_FLAGS) $^ -o web_build/$(WEB_TARGET).html
	@echo "Web build complete! Files in web_build/"
	@echo "Run a local server to test (e.g., python -m http.server 8000)"

clean:
	rm -f $(TARGET)

clean-web:
	rm -rf web_build

help:
	@echo "GoGame Build System"
	@echo "==================="
	@echo ""
	@echo "Targets:"
	@echo "  make              - Build and run desktop version (default)"
	@echo "  make $(TARGET)    - Build desktop executable only"
	@echo "  make web          - Build web version (requires Emscripten)"
	@echo "  make clean        - Remove desktop build files"
	@echo "  make clean-web    - Remove web build files"
	@echo "  make help         - Show this help message"
	@echo ""
	@echo "For web deployment, see GETTING_ONLINE.md"