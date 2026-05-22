# Shadow's Edge - Pure C++ 2D Physics Engine

A lightweight, custom-built 2D physics and collision engine written from scratch in native C++ using the **SFML 2.6** graphics framework. 

This project demonstrates the core mathematical mechanics behind video game physics loops—completely bypassing pre-made physics engines to handle manual kinematics, vector forces, and discrete collision resolution.

---

## 🚀 Engine Architecture & Features

* **Custom Kinematics Loop:** Implements real-time **velocity vectors**, acceleration due to **gravity** ($g$), and horizontal **drag/friction coefficients** (`velocityX *= 0.8f`) to handle organic, sliding movement.
* **AABB Collision Detection:** Uses **Axis-Aligned Bounding Box** calculation layers to scan for physical entity intersections between the player and environmental structures.
* **Discrete Collision Resolution:** Automatically intercepts bounding intersections on the $Y$-axis, instantly correcting penetration depth to prevent the player from falling through solid surfaces.
* **Fixed Delta Time Execution:** Hardcoded physics updates mapped step-by-step to a structured frame timing value (`deltaTime = 0.016f`) for predictable, hardware-independent behavior.

---

## 📂 Project Directory Structure

```text
SHADOW'S EDGE/
├── .gitignore          # Prevents tracking of build artifacts (.dll and .exe)
├── README.md           # Documentation
├── SFML/               # Local SFML binary toolchain framework (Version 2.6)
│   ├── bin/            # Dynamic Link Libraries (.dll)
│   ├── include/        # SFML C++ blueprint headers
│   └── lib/            # Static compilation wrappers (.a)
└── src_cpp/            # Engine source files
    ├── include/
    │   └── Player.hpp  # Physics structure & AABB boundaries
    └── src/
        ├── Player.cpp  # Kinematic equations & collision handling
        └── test.cpp    # Active SFML rendering and loop control thread



🔧 Installation & Compilation Guide
1. Prerequisites & Dependencies
Compiler: GCC MinGW-w64 SEH (64-bit) matching GCC 13/14/15 toolchains.

Graphics Framework: SFML 2.6.1 GCC MinGW (64-bit) binaries.

2. Runtime Environment Setup
Windows graphics applications require the core compilation and framework shared libraries to sit right alongside the compiled binary at runtime to execute successfully.

Ensure the following files have been copied from your compiler's toolchain (C:\msys64\mingw64\bin\) and your local SFML/bin/ folder directly into your src_cpp/ execution space:

sfml-graphics-2.dll

sfml-window-2.dll

sfml-system-2.dll

libgcc_s_seh-1.dll

libstdc++-6.dll

3. Compiling via Terminal
To compile and build the native machine code application from the root project parent directory, open your terminal and run:

Bash:
g++ src_cpp/src/Player.cpp src_cpp/src/test.cpp -I src_cpp/include -I SFML/include -L SFML/lib -lsfml-graphics -lsfml-window -lsfml-system -o src_cpp/test_engine.exe
4. Running the Application
Change directories into your source folder where your executable and environment DLLs reside, then launch the engine binary:

PowerShell
cd src_cpp
./test_engine.exe
🎮 Game Controls
D: Apply rightward velocity vector force (Accelerates position along positive X).

A: Apply leftward velocity vector force (Accelerates position along negative X).

X-Button: Safely break out of threads, flush the render cycle, and shut down the window buffer context cleanly.        