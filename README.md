# GoGame (C++ / Raylib)

A modern implementation of the traditional **Go** board game featuring a simple, responsive UI built with **C++** and the **Raylib** graphics library.

---

## Table of Contents
- [Overview](#overview)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Building](#building)
- [Running](#running)
- [Project Structure](#project-structure)
- [Controls](#controls)
- [Features](#features)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)
- [License](#license)

---

## Overview

GoGame is a minimal and educational implementation of the classic **Go (Weiqi, Baduk)** board game.
It focuses on clarity and modularity, demonstrating a clean object-oriented architecture and using Raylib for rendering.

**Key highlights**
- Interactive Go board with stone placement
- Capture detection and basic scoring
- Undo/redo command system
- AI bot using Minimax and alpha-beta pruning
- Rendering and input handled by Raylib

---

## Prerequisites

**Windows** — required tools:

- **C++20 Compatible Compiler:** GCC or Clang
- **Raylib:** Graphics and audio library

**Other Platforms**

The project is primarily configured for Windows but can be adapted for Linux/macOS with appropriate dependency paths.

---

## Installation

### 1. Install MSYS2
```bash
Download from https://www.msys2.org
```

### 2. Install Dependencies
#### Open MSYS2 UCRT64
```bash
# Make sure to update MSYS2
pacman -Syu

# Install Raylib graphics library
pacman -S mingw-w64-x86_64-raylib

# Install make
pacman -S make

# Install gcc
pacman -S mingw-w64-x86_64-gcc
```

### 3. Clone the Repository
```bash
git clone https://github.com/khanh47/GoGame.git
cd GoGame
```

## Building

The project uses a Makefile for compilation with automatic dependency detection.

### Quick Build & Run
```bash
make
```
This command will:
1. Compile all source files
2. Link with Raylib
3. Run the executable
4. Clean up the binary after execution

### Build Only (Without Running)
```bash
make compile
```

### Clean Build Files
```bash
make clean
```

## Running

After successful compilation:

### Option 1: Using Make (Recommended)
```bash
make
```

### Option 2: Direct Execution
```bash
./run
```

### Option 3: Build Then Run Separately
```bash
make compile
./run
```

## Project Structure

```
GoGame/
├── assets/                   # Game assets
│   ├── images/               # Background theme, stones and board textures
│   ├── fonts/                # Custom fonts
│   ├── musics/               # Audio files
│   └── sounds/               # Sound effects
├── include/                  # Header files
│   ├── AI/                   # Minimax and alpha-beta algorithm for bot
│   ├── Commands/             # Command pattern implementation
│   ├── Game/                 # Core game logic
│   ├── GameData/             # Game's data management
│   ├── GameStates/           # State pattern for game flow
│   ├── Menu/                 # Menu system (Composite pattern)
│   ├── Render/               # Rendering and view components
│   ├── Resource/             # Resource and settings management
│   └── Scene/                # Scene management
├── src/                      # Source files
│   ├── AI/                   # Minimax and alpha-beta algorithm for bot
│   ├── Commands/
│   ├── Game/
│   ├── GameData/
│   ├── GameStates/
│   ├── Menu/
│   ├── Render/
│   ├── Resource/
│   ├── Scene/
│   └── main.cpp              # Entry point
├── makefile                  # Build configuration
└── README.md                 # This file

```

## Controls

### Menu Navigation
- Mouse: Click to select menu items
- Hover: Visual feedback on interactive elements

### Game Controls
- Mouse Click: Place stones and pass turn
- Mouse Hover: Preview available moves
- Menu Button: Access in-game menu for save/load/undo/redo

## Features

### Core Gameplay
- **Go Rules Implementation:** Full support for japanese rule (e.g. liberties, captures, suicide rule, and territory scoring)
- **Board Sizes:** Supports **9×9**, **13×13**, and **19×19** boards  
- **Undo/Redo System:** Complete move history with branching support  

### Artificial Intelligence
- **Pluggable AI Engine:** Modular design that allows different evaluation strategies  
- **Heuristic Evaluation:** Focus on alive groups, atari detection, and game-phase-based tuning  
- **Time-Limited Calculation:** Adjustable computation time for AI move search

### User Interface
- **Dynamic Menus:** Context-sensitive navigation system
- **Multiple View Modes:** Button-based and list-based menu layouts
- **Animated Transitions:** Smooth stone placement and UI animations  
- **Responsive Layouts:** Automatically adapts to different resolutions and window sizes

### Technical Features
- **Modular Architecture:** Clean separation of Model, View, and Controller  
- **Design Patterns:** Command, State, Strategy, Composite, Singleton
- **Resource Management:** Efficient texture/music loading and caching 
- **Extensible Framework:** Easy to add new game modes, AI strategies, or UI panels  

## Troubleshooting
### 1. Raylib not found
Verify include and lib paths point to your Raylib installation:

```makefile
INCLUDE = -I"C:/msys64/ucrt64/include"
LIBS = -L"C:/msys64/ucrt64/lib" -lraylib -lopengl32 -lgdi32 -lwinmm

### Common Build Issues

#### 1. Raylib Not Found
```bash
# Verify Raylib installation
pacman -Qs raylib
# If not installed:
pacman -S mingw-w64-x86_64-raylib
```

#### 2. make Not Found
```bash
# Verify make installation
make --version
# If not installed:
pacman -S make

# Put make installation path in Environment Variables
where make
```

#### 3. Compiler Not Found
```bash
# Verify GCC/Clang
g++ --version
```

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/super-feature`)
3. Commit your changes (`git commit -m 'Add super feature'`)
4. Push to the branch (`git push origin feature/super-feature`)
5. Open a Pull Request

### Code Style
- Use C++20 standards
- Follow the existing naming conventions
- Document new classes and methods
- Maintain the established design patterns

### Testing
- Test builds on Windows and Linux before submitting
- Verify all menu interactions work correctly
- AI functionality has been tested and verified through gameplay
- Ensure no memory leaks with new features

## License

This project is part of an academic assignment for Introduction to Computer Science course.

## Acknowledgments

- **Raylib**: Graphics and audio library
- **HCMUS**: University of Science, VNU-HCM
- **Course**: Introduction to Computer Science (CS160)

---

For questions or issues, please open an issue on the GitHub repository or contact the development team.
