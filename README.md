# GoGame (C++ / Raylib)

A modern implementation of the traditional **Go** board game featuring a simple, responsive UI built with **C++** and the **Raylib** graphics library.

---

## Table of Contents
- [Overview](#overview)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
  - [1. Install MinGW](#1-install-mingw)
  - [2. Install Raylib](#2-install-raylib)
  - [3. Clone the repository](#3-clone-the-repository)
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

GoGame is a minimal and educational implementation of the classic **Go (Weiqi, Baduk)** board game for **Windows**.  
It focuses on clarity and modularity, demonstrating a clean object-oriented architecture and using Raylib for rendering.

**Key highlights**
- Interactive Go board with stone placement
- Turn-based logic for Black and White players
- Capture detection and basic scoring
- Modular code structure (ready for AI/UI extensions)
- Rendering and input handled by Raylib

---

## Prerequisites

**Windows** — required tools:

- **MinGW-w64** (C++17 compatible) — or MSYS2 toolchain  
- **Raylib** (MinGW build or compatible)  
- **make** (provided by MinGW/MSYS2)  
- **git** (optional, for cloning/pushing)

---

## Installation

### 1. Install MinGW
Install MinGW-w64 or use MSYS2. Ensure `g++` are available in your PATH:

```bash
g++ --version
```

### 2. Install MSYS2 (if not already installed)
```bash
Download from https://www.msys2.org
```

### 3. Install Dependencies
```bash
# Update the Package Database 
pacman -Syu
# Install Raylib graphics library
pacman -S mingw-w64-x86_64-raylib
```

### 4. Clone the Repository
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

### Manual Compilation
If you prefer manual compilation or need to debug build issues:

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
├── assets/               # Optional: images, sounds, screenshot
├── include/              # Header files
│   ├── Game.h
│   ├── Board.h
│   ├── UI.h
│   └── AI.h
├── src/                  # Source files
│   ├── main.cpp          # Entry point
│   ├── Game.cpp          # Core game logic
│   ├── Board.cpp
│   ├── UI.cpp
│   └── AI.cpp
├── Makefile              # Build configuration
└── README.md             # This file

```

## Controls

### Menu Navigation

## Features

## Troubleshooting
### 1. Raylib not found
Verify include and lib paths point to your Raylib installation:

```makefile
INCLUDE = -I"C:/msys64/mingw64/include"
LIBS = -L"C:/msys64/mingw64/lib" -lraylib -lopengl32 -lgdi32 -lwinmm

### Common Build Issues

#### 1. Raylib Not Found
```bash
# Verify Raylib installation
brew list raylib
# If not installed:
brew install raylib
```

#### 2. Compiler Not Found
```bash
# Verify GCC/Clang
g++ --version
```

### Runtime Issues

### Platform-Specific Notes

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Code Style
- Use C++20 standards
- Follow the existing naming conventions
- Document new classes and methods
- Maintain the established design patterns

## License

This project is part of an academic assignment for Object-Oriented Programming course.

## Acknowledgments

- **Raylib**: Graphics and audio library
- **HCMUS**: University of Science, VNU-HCM
- **Course**: Object-Oriented Programming (OOP)

---

For questions or issues, please open an issue on the GitHub repository or contact the development team.