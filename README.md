# Coup Game - C++ Project

## Game Description
Coup is a strategic board game for 2-6 players competing against each other to be the last one standing.
Each player takes on a unique role with special abilities: Governor, Spy, Baron, General, Judge, and Merchant.
The goal is to eliminate all other players through various actions such as gathering coins, taxation, bribery, arrest, sanctions, and coups.

## Game Architecture
The game is built using Object-Oriented Programming (OOP) with the following structure:

### Class Structure
- **Base class `Player`** - Semi-abstract base class with shared functionality for all players
- **Role classes** - Inherit from `Player` with unique abilities for each role:
  - `Governor` - Can get 3 coins from tax and cancel others' tax actions
  - `Spy` - Can view others' coins and prevent arrest actions
  - `Baron` - Can invest (3→6 coins) and receive compensation when sanctioned
  - `General` - Can prevent coups and receive compensation when arrested
  - `Judge` - Can cancel bribe actions and collect compensation when sanctioned
  - `Merchant` - Gets bonus coin and pays double when arrested
- **`Game` class with Singleton pattern** - Manages game state, turns, and actions
- **`PlayerFactory`** - Provides functions to create players of different roles through the game

### File Structure
```
├── GAME/                    # Game engine
│   ├── game.hpp/cpp        # Game class (Singleton)
│   └── demo.cpp            # Complete demo file
├── PLAYER/                 # Player classes
│   ├── player.hpp/cpp      # Base Player class
│   ├── governor.hpp/cpp    # Governor class
│   ├── spy.hpp/cpp         # Spy class
│   ├── baron.hpp/cpp       # Baron class
│   ├── general.hpp/cpp     # General class
│   ├── judge.hpp/cpp       # Judge class
│   ├── merchant.hpp/cpp    # Merchant class
│   └── PlayerFactory.hpp/cpp # Factory for creating players
├── GUI/                    # Graphical interface
│   ├── GameGUI.hpp/cpp     # Main GUI class (SFML)
│   ├── gui_demo.cpp        # GUI demonstration
│   └── GUI_STRATEGY.md     # GUI strategy document
├── TEST/                   # Unit tests
│   ├── doctest.h          # Testing library
│   ├── testGame.cpp       # Game class tests
│   ├── testPlayer.cpp     # Player class tests
│   └── testRole.cpp       # Role-specific tests
├── assets/                # Graphic resources
│   └── fonts/arial.ttf    # Font for GUI
└── makefile               # Compilation file

```

## Roles and Unique Abilities

### Basic Actions (Available to all players)
- **Gather** - Player gets one coin from the treasury
- **Tax** - Player gets two coins from the treasury (or more depending on role)
- **Bribe** - Pay 4 coins to perform an additional action in the turn
- **Arrest** - Take a coin from another player
- **Sanction** - Prevent economic actions from another player (costs 3 coins)
- **Coup** - Remove another player from the game (costs 7 coins)

### Unique Abilities by Role
1. **Governor**
   - Gets 3 coins instead of 2 from tax action
   - Can cancel tax actions of other players

2. **Spy**
   - Can view the number of coins of another player
   - Can prevent another player from using arrest action in their next turn

3. **Baron**
   - Can invest 3 coins and get 6 in return
   - If attacked with sanction, receives one coin as compensation

4. **General**
   - Can pay 5 coins to prevent a coup against another player
   - If hurt by arrest, gets back the coin that was taken

5. **Judge**
   - Can cancel bribe actions of other players
   - If attacked with sanction, the attacking player pays an additional coin to the treasury

6. **Merchant**
   - If starts turn with 3+ coins, gets an additional coin
   - If attacked with arrest, pays 2 coins to treasury instead of losing 1 to attacker

## Special Game Rules
- Player with 10+ coins at the start of their turn must perform a coup
- Cannot arrest the same player twice in a row
- Undo actions are performed "in real time" until the canceling player's turn returns
- Multiple players can have the same role

## Implementation Features

### Design Principles
- **Singleton Pattern** - Game class ensures a single active game
- **Factory Pattern** - Player creation through PlayerFactory
- **Polymorphism** - All roles inherit from Player with virtual functions

### Graphical User Interface (GUI)
- Based on **SFML** library for graphics and windows
- Simple and intuitive interface with screen flow:
  - Welcome and greeting screen
  - Player setup and role selection screen
  - Main game screen with all actions
  - Winner announcement screen
- Support for up to 6 players on shared screen
- Real-time display of coins, turns, and game state

### Testing and Safety
- **Comprehensive unit tests** with doctest library
- Tests for all roles, actions, and edge cases
- **Memory leak detection** with Valgrind
- Comprehensive exception handling and input validation
- Validation of illegal actions

## Compilation and Execution Instructions
Use the included Makefile with the following commands:

```bash
# Run game demo (console)
make demo

# Run graphical interface
make gui

# Run unit tests
make test

# Memory leak detection with Valgrind
make valgrind

# Compile all components
make all

# Clean temporary files
make clean
```
## Working with Graphical Interface
To run the graphical interface:
```bash
make gui
```
The interface will guide you through steps:
1. **Welcome screen** - Click "Start New Game"
2. **Player setup** - Enter names and select roles
3. **Game** - Choose actions according to turn
4. **End** - Display winner

### System Requirements
- **C++ compiler** with C++17 support or higher
- **SFML library** for graphical interface:
  ```bash
  # Ubuntu/Debian
  sudo apt-get install libsfml-dev
  ```
- **Valgrind** for memory leak detection (recommended)

## Project Status and Completions

### Fully Implemented ✅
- **All player classes** - All 6 roles with all unique abilities
- **Game engine** - Turn management, game state, complete validation
- **Comprehensive unit tests** - Coverage of all functions and edge cases
- **Functional graphical interface** - Complete GUI with SFML
- **Examples and demonstrations** - Complete and functional demo
- **Makefile** - Support for all required options

### Additional Features Added 🚀
1. **Interface creation change** - Using Factory through game instead of direct constructors
2. **Absolute Singleton** - Game as true Singleton instead of passing reference
3. **Modern interface** - Using SFML 
4. **Improved file structure** - Clear division into directories by function
5. **Enum classes** for roles and action types

### Code Quality 🔧
- **Strict C++17 standards**
- **No memory leaks** (verified with Valgrind)
- **Comprehensive exception handling** in all functions
- **Documented and organized code** with detailed comments
- **Modular structure** with clear separation of responsibilities

## Contribution and Future Development
The project is built modularly and allows future extensions:
- Adding new roles
- Adding new actions
- Upgrading the graphical interface
- Network support (multiplayer)

---

**Author:** idocohen963@gmail.com  
**Project:** CPP_EX3 - Coup Game  
**Date:** 2025