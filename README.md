
# GoGame (C++ / raylib)

A simplified Go game implementation in C++ using raylib.  
Board size: 9x9 (can be changed).  
Features:
- 2-player local mode
- Player vs AI with three difficulty levels: easy (random), medium (minimax shallow), hard (alpha-beta)
- Undo / Redo
- Save / Load (JSON)
- Basic UI: board, turn indicator, simple buttons

**Raylib installation**
This project expects raylib headers and libraries in: `D:/Tools/msys64/ucrt64`
If your raylib is at the path you told me, the included Makefile and CMakeLists.txt are preconfigured.

## Build (Makefile)

In MSYS2 UCRT64 shell (or Windows cmd if you adapt paths):

```bash
cd /mnt/data/GoGame
make
# or: make RAYLIB_DIR="D:/Tools/msys64/ucrt64"
./GoGame.exe
```

## Build (CMake)

```bash
mkdir build
cd build
cmake -DCMAKE_PREFIX_PATH="D:/Tools/msys64/ucrt64" ..
cmake --build .
./GoGame.exe
```

## Files
- src/: C++ sources
- include/: headers
- assets/: placeholder for sounds/images (empty)

## Notes
- This is a functional, simplified Go implementation intended for the project requirements. Rules implemented: placing stones, capture by liberties, basic Ko prevention, simple end scoring (area-like). It is not a full competition-grade Go engine.
- AI depths are limited to remain responsive. Modify AI depth in `AI.cpp` if you want stronger play.
