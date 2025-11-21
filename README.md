# Digital Exodus

A Matrix-inspired procedural storytelling text-based RPG demonstrating Data Structures and Patterns concepts.

## Project Overview

**Course:** COS30008 Data Structures and Patterns (2025 S2)
**Assessment:** Programming Project 1

Digital Exodus is a C++ console application where you play as an "Awakened" entity within a vast simulation called "The Grid". Navigate through sectors, engage in combat with Sentinels, hack terminals, and work toward escaping to freedom.

## Demonstrated Concepts

### Object-Oriented Programming
1. **Inheritance and Derived Classes** - `Entity` base class with `Player`, `Sentinel`, `Awakened` derived classes
2. **Polymorphism** - Virtual `performAction()` method with different implementations per class

### Composite Data Structures
3. **Hash Table** - `ItemDatabase` for O(1) item lookup by code
4. **Singly Linked List** - `EventLog` for game event history (LIFO insertion)
5. **Doubly Linked List** - `GridMap` for 4-directional sector navigation

### Abstract Data Types
6. **Stack** - `ActionStack` for undo system in hacking minigame
7. **Queue** - `CombatQueue` for turn-based combat turn order
8. **Tree** - `AbilityTree` for skill progression with prerequisites

### Design Patterns
9. **Iterator** - `InventoryIterator` for filtered inventory traversal
10. **Observer** - AI Storyteller (`Overseer`) observing player state and triggering events

### Additional Features
- **SFML Audio** - Background music and sound effects (optional)
- **File Operations** - Save/Load game system
- **AI Storyteller** - Influences combat, events, and environment based on player health and progress

## Building the Project

### Linux/macOS (g++)
```bash
# Build without audio
make

# Build with SFML audio
make sfml

# Run the game
./DigitalExodus

# Clean build files
make clean
```

### Windows (Visual Studio)
1. Create a new Empty C++ Console Application
2. Add all `.h` files from `include/` to Header Files
3. Add all `.cpp` files from `src/` to Source Files
4. Set C++ Language Standard to C++11 or later
5. Build and run

For SFML audio support:
- Download SFML from https://www.sfml-dev.org/
- Add SFML paths and link libraries
- Define `SFML_AVAILABLE` preprocessor macro

## Project Structure

```
TheArchitect/
├── include/           # Header files
│   ├── Entity.h       # Inheritance & Polymorphism
│   ├── HashTable.h    # Hash Table implementation
│   ├── LinkedList.h   # Singly & Doubly Linked Lists
│   ├── Stack.h        # Stack implementation
│   ├── Queue.h        # Queue implementation
│   ├── Tree.h         # Tree implementation
│   ├── Iterator.h     # Iterator pattern
│   ├── Observer.h     # Observer pattern (AI Storyteller)
│   ├── Audio.h        # SFML audio wrapper
│   ├── SaveLoad.h     # File operations
│   └── Game.h         # Main game systems
├── src/               # Implementation files
│   ├── Entity.cpp
│   ├── HashTable.cpp
│   ├── LinkedList.cpp
│   ├── Stack.cpp
│   ├── Queue.cpp
│   ├── Tree.cpp
│   ├── Iterator.cpp
│   ├── Observer.cpp
│   ├── Audio.cpp
│   ├── SaveLoad.cpp
│   ├── Game.cpp
│   └── main.cpp
├── audio/             # Audio files (optional)
├── GAME_DESIGN_PLAN.md
├── Makefile
└── README.md
```

## Gameplay Features

- **Exploration** - Navigate through interconnected sectors of The Grid
- **Combat** - Turn-based battles with Sentinels using queue-based turn order
- **Hacking** - Minigame with undo capability using stack data structure
- **Character Progression** - Skill tree with prerequisite-based unlocking
- **Inventory Management** - Item system with filtered iteration
- **Save/Load** - Persistent game state through file operations
- **AI Storyteller** - Dynamic event generation based on player state

## Game Commands

- `N/S/E/W` - Move in cardinal directions
- `I` - View inventory
- `A` - View ability tree
- `L` - View event log
- `H` - Hack terminal (when available)
- `R` - Rest to recover health
- `S` - Save game
- `Q` - Quit to menu

## License

See LICENSE file for details.
