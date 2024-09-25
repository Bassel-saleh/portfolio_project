# 3D Game Engine with SDL2

This project is a 3D game engine built using **SDL2** and **C**. It features a raycasting engine similar to the rendering style of *Wolfenstein 3D*. The game includes movement, enemy spawning, and weapon rendering.

## Features

- **Raycasting Rendering**: Uses raycasting to render walls, floor, and ceilings based on the player's position.
- **Player Movement**: Control player movement with keyboard inputs (WASD) and rotate view using arrow keys.
- **Map Rendering**: 2D grid map with textured walls and ground.
- **Enemy Spawning**: Enemies spawn at intervals and can be rendered in 3D space.
- **Weapon Rendering**: Display a weapon at the bottom of the screen as the player moves through the map.

## Table of Contents

1. [Installation](#installation)
2. [Usage](#usage)
3. [Controls](#controls)
4. [Map Format](#map-format)
5. [Game Structure](#game-structure)

## Installation

1. Clone the repository:

    ```bash
    git clone https://github.com/yourusername/3D-Game-Engine.git
    cd 3D-Game-Engine
    ```

2. Install the SDL2 library and its dependencies:

    - Ubuntu:
      ```bash
      sudo apt-get install libsdl2-dev libsdl2-image-dev
      ```

3. Build the project:

    ```bash
    make
    ```

## Usage

Once the project is built, you can start the game by running the executable:

```bash
./game
```

## Controls
	W: Move forward
	S: Move backward
	A: Strafe left
	D: Strafe right
	Left/Right Arrow Keys: Rotate view
	M: Toggle map view
	ESC: Exit the game

## Map Format
The map is a 16x16 grid stored in a .txt file. Each position in the map is represented by either:

	1: A wall
	0: An empty space

## Game Structure
The game is built using the following main components:

	Player: The player has a position, direction, and field of view (FOV).
	Raycasting: Rays are cast from the player’s position to calculate wall distances and render the 3D environment.
	Enemy: Enemies are rendered in the game, and they spawn periodically.
	Weapon: A weapon is rendered at the bottom of the screen to enhance the 3D effect.

## Key Data Structures
	State: Manages the game's window, renderer, textures, and game loop state.
	Player: Contains the player’s position, direction, and view plane.
	Enemy: Handles enemy position and texture.
	Vec2F and Vec2I: Handle 2D vectors for floating-point and integer coordinates.
