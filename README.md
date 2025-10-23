# Competitive Snake

A modern multiplayer snake game built with C++ and Raylib, featuring both local multiplayer and AI opponent modes.

## Features

### Game Modes
- **Player vs Player**: Compete against a friend in local multiplayer
  - Player 1: WASD controls
  - Player 2: Arrow key controls
  - Simultaneous ready-up system - both players must press a direction key to start
  
- **Player vs AI**: Challenge a smart AI opponent
  - AI uses Manhattan distance pathfinding
  - Avoids walls and collision detection
  - Competitive food-seeking behavior

### Scene Management
- Unity-style scene system with SceneManager
- Main menu with animated selection
- Ready-up screens for each game mode
- Scene transitions with build indices

### Technical Features
- Smooth gameplay at 165 FPS target
- Music streaming support
- Collision detection for walls, food, and snake bodies
- Score tracking for both players
- Custom snake positioning system
- Real-time direction changes with reverse-prevention

## Building

### Prerequisites
- C++17 compatible compiler (clang++ recommended)
- Raylib 5.5
- Make

### Compilation
```bash
make
```

### Running
```bash
make run
```

### Cleaning Build
```bash
make clean
```

## Project Structure
```
CompetitiveSnake/
├── include/           # Header files
│   ├── Game.hpp
│   ├── Snake.hpp
│   ├── Food.hpp
│   ├── Global.hpp
│   ├── Scene.hpp
│   ├── SceneManager.hpp
│   ├── MainMenuScene.hpp
│   ├── GameScene.hpp
│   └── AIGameScene.hpp
├── src/              # Implementation files
│   ├── main.cpp
│   ├── Game.cpp
│   ├── Snake.cpp
│   ├── Food.cpp
│   ├── Global.cpp
│   ├── Scene.cpp
│   ├── SceneManager.cpp
│   ├── MainMenuScene.cpp
│   ├── GameScene.cpp
│   └── AIGameScene.cpp
├── Assets/           # Game assets
│   ├── Graphics/
│   └── Sounds/
├── Makefile
└── README.md
```

## Controls

### Main Menu
- **W/S** or **Arrow Keys**: Navigate menu
- **Enter**: Select option
- **ESC**: Quit game

### Player vs Player Mode
- **Player 1**: WASD keys
- **Player 2**: Arrow keys
- **ESC**: Return to main menu

### Player vs AI Mode
- **WASD**: Control your snake
- **ESC**: Return to main menu

## Gameplay

1. Launch the game to see the main menu
2. Select your preferred game mode
3. Wait for the ready-up screen
4. Press your direction keys to start
5. Collect food to grow your snake
6. Avoid walls and other snakes
7. Score points by eating food
8. Return to the main menu with ESC

## Development

Built with:
- **Language**: C++17
- **Framework**: Raylib 5.5
- **Compiler**: Clang++
- **Build System**: Make
- **Architecture**: Scene-based management system

## Future Enhancements
- [ ] Pause menu functionality
- [ ] Options menu (sound, controls customization)
- [ ] High score tracking
- [ ] Multiple AI difficulty levels
- [ ] Network multiplayer
- [ ] Additional power-ups

## Author
Navi

## License
This project is open source and available for educational purposes.