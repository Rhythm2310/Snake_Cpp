# Snake_Cpp

Welcome to **Snake_Cpp**, a simple yet engaging implementation of the classic **Snake Game** written in C++ using the SDL2 library. This game allows players to control a snake on a grid, eat food to grow, and avoid crashing into walls or itself. It features particle effects, a splash screen, and smooth gameplay with SDL2's powerful rendering and input handling capabilities.

---

## Table of Contents

1. [Overview](#overview)
2. [Dependencies](#dependencies)
3. [Game Constants](#game-constants)
4. [Game Logic](#game-logic)
    1. [Snake Movement](#snake-movement)
    2. [Food](#food)
    3. [Collision Detection](#collision-detection)
5. [Particle System](#particle-system)
6. [Rendering](#rendering)
7. [Handling User Input](#handling-user-input)
8. [Game Over Logic](#game-over-logic)
9. [Splash Screen](#splash-screen)
10. [How to Build and Run](#how-to-build-and-run)
    1. [Windows Setup](#windows-setup)
    2. [Linux/macOS Setup](#linuxmacos-setup)
11. [License](#license)

---

## Overview

This C++ implementation of **Snake Game** uses **SDL2** for graphics rendering, input handling, and text display. The game follows the traditional rules where you control a snake, eat food to grow, and avoid obstacles (walls or your own body). The game includes a particle effect when food is eaten, and a splash screen that greets the player at the start.

---

## Dependencies

To build and run the game, you'll need the following dependencies:

- **SDL2**: A cross-platform multimedia library for handling window creation, graphics rendering, and event management.
- **SDL2_ttf**: A library for handling TrueType fonts, used for rendering text in the game (e.g., score display).

You can install these dependencies using your system’s package manager or by downloading and building them manually.

### For Linux (Ubuntu/Debian-based):
```bash
sudo apt-get update
sudo apt-get install libsdl2-dev libsdl2-ttf-dev
```

### For macOS (using Homebrew):
```bash
brew install sdl2 sdl2_ttf
```

---

## Game Constants

Several constants are defined for controlling the game's behavior:

```cpp
// Grid size and screen dimensions
const int GRID_SIZE = 40;                // Grid cell size (in pixels)
const int SCREEN_WIDTH = 800;             // Width of the game window (in pixels)
const int SCREEN_HEIGHT = 600;            // Height of the game window (in pixels)
const int GRID_WIDTH = SCREEN_WIDTH / GRID_SIZE;  // Number of grid columns
const int GRID_HEIGHT = SCREEN_HEIGHT / GRID_SIZE; // Number of grid rows

// Frame rate constants
const int TARGET_FPS = 60;               // Target frames per second
const int FRAME_DELAY = 1000 / TARGET_FPS; // Frame delay for consistent FPS
const int SNAKE_UPDATE_DELAY = 15;       // Delay before updating snake's position

// Particle system and splash screen duration
const int MAX_PARTICLES = 200;           // Max number of particles in the system
const int SPLASH_SCREEN_DURATION = 3000; // Splash screen duration (in milliseconds)
```

---

## Game Logic

### Snake Movement
The snake moves on a grid where the player can control its direction using the arrow keys. Each time the snake moves, a new head is added to the front, and if the snake does not eat food, the tail is removed.

- The snake moves one grid cell at a time.
- The movement is controlled by the `UP`, `DOWN`, `LEFT`, and `RIGHT` keys.

### Food
Food spawns randomly on the grid and is represented as a red square. When the snake eats food, its length increases, and the score is updated by 10 points.

### Collision Detection
The game checks for two types of collisions:
1. **Wall collision**: If the snake's head collides with the screen's boundary, the game ends.
2. **Self-collision**: If the snake's head collides with any part of its body, the game ends.

---

## Particle System

Particles are generated when the snake eats food, creating a burst effect. Each particle has:
- A position (`x`, `y`),
- A velocity (`vx`, `vy`),
- A lifetime that determines how long it remains on the screen,
- An age that increments over time.

Particles fade out as their lifetime decreases, adding a nice visual effect to food consumption.

---

## Rendering

The rendering system is responsible for drawing all game elements:
1. **Grid Lines**: To visualize the grid, grid lines are drawn on the screen.
2. **Snake**: The snake is rendered as a series of rectangles.
3. **Food**: The food is represented as a red rectangle.
4. **Particles**: Particles are drawn as small yellow rectangles, with alpha transparency decreasing over time.
5. **Score**: The score is displayed at the top-left corner using SDL2's `TTF` text rendering.

---

## Handling User Input

The game listens for key events using `SDL_PollEvent`:
- **Arrow keys**: Change the snake's direction.
- **R key**: Restart the game after it ends.

---

## Game Over Logic

When a collision is detected (either with the wall or the snake’s body), the game enters a "Game Over" state. The game over screen displays the message and the final score, prompting the player to press `R` to restart.

---

## Splash Screen

At the start of the game, a splash screen is displayed for 3 seconds. It shows a welcome message and a brief introduction. The splash screen is rendered using a texture created from SDL2's font rendering system.

---

## How to Build and Run

### Windows Setup

To build the game on **Windows**, you will need **CMake** and **MinGW** or **MSVC**. Follow the steps below to build and run the game:

#### Step 1: Install Dependencies

Download and install **SDL2** and **SDL2_ttf** for Windows:
- SDL2: [https://www.libsdl.org/download-2.0.php](https://www.libsdl.org/download-2.0.php)
- SDL2_ttf: [https://www.libsdl.org/projects/SDL_ttf/](https://www.libsdl.org/projects/SDL_ttf/)

#### Step 2: Build with CMake

1. Open a **command prompt** and navigate to the root of the project.
2. Create a `build` directory:
   ```bash
   mkdir build
   cd build
   ```
3. Generate the Visual Studio project (replace `Visual Studio 16 2019` with your version):
   ```bash
   cmake .. -G "Visual Studio 16 2019"
   ```
4. Build the project:
   ```bash
   cmake --build . --config Release
   ```
5. Run the compiled game.

#### Step 3: Build with MinGW (optional)

If you prefer MinGW, use this instead:

```bash
cmake .. -G "MinGW Makefiles"
mingw32-make
```

---

### Linux/macOS Setup

To build the game on **Linux** or **macOS**, follow these steps:

#### Step 1: Install Dependencies

- **On Linux (Ubuntu/Debian-based)**:
  ```bash
  sudo apt-get update
  sudo apt-get install libsdl2-dev libsdl2-ttf-dev cmake
  ```

- **On macOS** (using Homebrew):
  ```bash
  brew install sdl2 sdl2_ttf cmake
  ```

#### Step 2: Build with CMake and Ninja (optional)

1. Install **Ninja**:
   ```bash
   sudo apt install ninja-build  # On Linux
   brew install ninja            # On macOS
   ```

2. Create a build directory and run CMake:
   ```bash
   mkdir build
   cd build
   cmake .. -G Ninja
   ninja
   ```

#### Step 3: Build with Make (alternative)

For a standard **Make** build:
```bash
mkdir build
cd build
cmake ..
make
```

#### Step 4: Run the Game

After building, you can run the game using:
```bash
./Snake_Cpp
```

---

## License

This project is licensed under the **GNU General Public License v3.0 (GPL-3.0)**. You can freely use, modify, and distribute the code under the terms of this license. See the [LICENSE](LICENSE) file for more details.

---

### Contributions
Feel free to contribute by creating a pull request or opening an issue for any bugs or feature requests!
