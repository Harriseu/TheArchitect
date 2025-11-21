/*******************************************************************************
 * main.cpp - Digital Exodus Entry Point
 *
 * DIGITAL EXODUS: A Matrix-Inspired Procedural Storytelling Game
 *
 * This program demonstrates the following Data Structures and Patterns concepts:
 *
 * 1. INHERITANCE & DERIVED CLASSES (Entity.h/cpp)
 *    - Entity base class with Player, Sentinel, Awakened derived classes
 *
 * 2. POLYMORPHISM (Entity.h/cpp)
 *    - Virtual performAction() method overridden in each derived class
 *
 * 3. HASH TABLE (HashTable.h/cpp)
 *    - ItemDatabase for O(1) item lookup by code
 *
 * 4. SINGLY LINKED LIST (LinkedList.h/cpp)
 *    - EventLog for game event history
 *
 * 5. DOUBLY LINKED LIST (LinkedList.h/cpp)
 *    - GridMap for 4-directional sector navigation
 *
 * 6. STACK (Stack.h/cpp)
 *    - ActionStack for undo system in hacking minigame
 *
 * 7. QUEUE (Queue.h/cpp)
 *    - CombatQueue for turn-based combat system
 *
 * 8. TREE (Tree.h/cpp)
 *    - AbilityTree for skill progression with prerequisites
 *
 * 9. ITERATOR PATTERN (Iterator.h/cpp)
 *    - InventoryIterator for traversing inventory with filters
 *
 * 10. OBSERVER PATTERN (Observer.h/cpp)
 *     - AI Storyteller (Overseer) observing player state and triggering events
 *
 * Additional Requirements:
 * - SFML Audio integration (Audio.h/cpp)
 * - File save/load system (SaveLoad.h/cpp)
 * - AI Storyteller influences 3+ aspects based on 2+ criteria
 *
 * Author: Digital Exodus Project
 * Course: COS30008 Data Structures and Patterns
 * Semester: 2025 S2
 ******************************************************************************/

#include <iostream>
#include <cstdlib>
#include <ctime>

#include "../include/Game.h"

/*******************************************************************************
 * MAIN FUNCTION
 *
 * Entry point for Digital Exodus.
 * Creates and runs the main game instance.
 ******************************************************************************/
int main(int argc, char* argv[]) {
    // Seed random number generator
    srand(static_cast<unsigned int>(time(nullptr)));

    std::cout << "Initializing Digital Exodus..." << std::endl;
    std::cout << std::endl;

    // Create and run game
    Game game;
    game.run();

    std::cout << "\nDigital Exodus terminated." << std::endl;

    return 0;
}

/*******************************************************************************
 * CONCEPT DEMONSTRATION NOTES
 *
 * When demonstrating for the video, show the following:
 *
 * 1. INHERITANCE & POLYMORPHISM:
 *    - Show Entity, Player, Sentinel, Awakened classes
 *    - Demonstrate how performAction() behaves differently for each
 *
 * 2. HASH TABLE:
 *    - Show item lookups in inventory/database
 *    - Demonstrate O(1) access time for items
 *
 * 3. SINGLY LINKED LIST:
 *    - Show event log adding and displaying events
 *    - Recent events displayed first (head insertion)
 *
 * 4. DOUBLY LINKED LIST:
 *    - Navigate through sectors (N/S/E/W)
 *    - Show bidirectional movement capability
 *
 * 5. STACK:
 *    - Use hacking minigame
 *    - Demonstrate undo with revert tokens (LIFO)
 *
 * 6. QUEUE:
 *    - Enter combat
 *    - Show turn order based on speed (FIFO)
 *
 * 7. TREE:
 *    - View ability tree
 *    - Show prerequisite requirements for unlocking
 *
 * 8. ITERATOR:
 *    - Filter inventory by type
 *    - Demonstrate traversal without exposing structure
 *
 * 9. OBSERVER:
 *    - Play and observe AI Storyteller events
 *    - Show how player state triggers different responses
 *
 * 10. FILE OPERATIONS:
 *     - Save game
 *     - Load game
 *     - Show save file created
 *
 * 11. AUDIO:
 *     - If SFML is available, demonstrate sound effects
 *     - Show audio status in options
 ******************************************************************************/
