# Shadow's Edge 🌃

A atmospheric, neon-noir 2D side-scrolling prototype built from scratch in C++ using the Simple and Fast Multimedia Library (SFML). Take control of "The Messenger," navigating a moody urban environment with physics-driven movement, jumping mechanics, and state-driven animations.

---

## 📁 Project Architecture

The workspace is organized with a clear separation between source code, system headers, compiled binaries, and multimedia assets:

```text
Shadow's Edge/
├── Assets/
│   ├── Background_img.jpg    # 1280x720 environment backdrop
│   └── hero_sheet.png        # 669x373 transparent character animation grid
└── src_cpp/
    ├── include/
    │   └── SFML/             # Local SFML development headers & binaries
    │       ├── bin/          # System Dynamic Link Libraries (.dll)
    │       ├── include/      # Engine header files (.hpp)
    │       └── lib/          # Compilation static library files (.a)
    └── src/
        └── test.cpp          # Core game loop and application source code
🛠️ Features ImplementedAsset Pipeline: 

Smooth loading of textures with dynamic scaling to fit a 1280x720 viewport.Sprite Sheet Slicing: Automated frame cropping using sf::IntRect designed for custom $67 \times 93$ pixel frame sizes.State Engine: Enum-driven player states (IDLE, WALKING, JUMPING, ATTACKING) that alter active sprite textures on the fly.Physics Engine: Real-time vertical velocity calculations integrated with constant gravity thresholds and collision checks for sidewalk boundaries.Responsive Animation: Custom sf::Clock frame timers running at 100ms intervals to handle smooth direction flips, walks, and combat triggers.

💻 Compilation and ExecutionThe project uses the UCRT64 MinGW-w64 compiler suite toolchain via MSYS64 on Windows.

To compile the source code, link the SFML components, and launch the standalone binary executable, execute the following chained command block from inside the src_cpp directory using PowerShell:PowerShell# Navigate to the workspace compilation context if needed: cd src_cpp

g++ -c src/test.cpp -I include/SFML/include; g++ test.o -o ShadowsEdge.exe -L include/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system; .\ShadowsEdge.exe


Command Parameter Breakdown:-I include/SFML/include: Tells the preprocessor where to find the <SFML/Graphics.hpp> style library files.-L include/SFML/lib: Tells the linker where the binary library engines are stored locally.-lsfml-graphics -lsfml-window -lsfml-system: Links the essential graphics rendering, OS window management, and system clock modules.


🎮 Game Controls
|----------------------------------------------------------------------------------|
|Key Bindings:             |         Action Performance:                           |
|--------------------------|-------------------------------------------------------|
|A / LEFT ARROW            |          Move Left (Flips character orientation)      |
|D / Right Arrow           |          Move Right (Marches down the street)         |
|Spacebar / Up Arrow       |          Jump (Engages gravity physics curves)        |
|F KEY                     |          Unsheathe blade & execute light sword slash  |
|----------------------------------------------------------------------------------|
