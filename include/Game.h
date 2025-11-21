/*******************************************************************************
 * Game.h - Main Game Systems and Loop
 *
 * This module contains the main game class and core systems:
 * - Game state management
 * - Main game loop
 * - Combat system
 * - Hacking minigame
 * - Menu systems
 *
 * Author: Alif Harriz Jeffery | 102782711
 * Course: COS30008 Data Structures and Patterns
 ******************************************************************************/

#ifndef GAME_H
#define GAME_H

#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "Entity.h"
#include "HashTable.h"
#include "LinkedList.h"
#include "Stack.h"
#include "Queue.h"
#include "Tree.h"
#include "Iterator.h"
#include "Observer.h"

/*******************************************************************************
 * Game State Enumeration
 ******************************************************************************/
enum class GameState {
    MAIN_MENU,
    NEW_GAME,
    LOAD_GAME,
    PLAYING,
    COMBAT,
    HACKING,
    INVENTORY,
    ABILITIES,
    EVENT_LOG,
    SAVE_MENU,
    OPTIONS,
    GAME_OVER,
    VICTORY,
    QUIT
};

/*******************************************************************************
 * Combat System
 *
 * Manages turn-based combat using Queue for turn order.
 ******************************************************************************/
class CombatSystem {
private:
    CombatQueue* turnQueue;     // Queue managing turn order
    Player* player;
    Entity** enemies;
    int enemyCount;
    Entity** allies;
    int allyCount;
    bool inCombat;
    int roundNumber;

public:
    CombatSystem();
    ~CombatSystem();

    // Combat initialization
    void initiateCombat(Player* player, Entity** enemies, int enemyCount,
                        Entity** allies = nullptr, int allyCount = 0);

    // Combat loop
    bool processTurn();         // Process one turn, return true if combat continues
    void endCombat();

    // Player actions
    void playerAttack(Entity* target);
    void playerDefend();
    void playerUseAbility(const std::string& abilityCode, Entity* target);
    void playerUseItem(const std::string& itemCode);
    void playerFlee();

    // AI actions
    void enemyTurn(Entity* enemy);
    void allyTurn(Entity* ally);

    // Status checks
    bool isVictory() const;
    bool isDefeat() const;
    bool isInCombat() const;

    // Display
    void displayCombatStatus() const;
    void displayTurnOrder() const;
    void displayPlayerOptions() const;

    // Rewards
    int calculateExperienceReward() const;
    Item* rollLootDrop() const;
};

/*******************************************************************************
 * Hacking Minigame
 *
 * Uses Stack for undo system during code injection.
 ******************************************************************************/
class HackingSystem {
private:
    ActionStack* actionHistory; // Stack for undo functionality
    int targetSecurity;         // Security level to overcome
    int currentProgress;        // Hacking progress
    int maxAttempts;            // Maximum allowed attempts
    int attemptsUsed;
    bool hackingActive;
    bool hackSucceeded;

public:
    HackingSystem();
    ~HackingSystem();

    // Hacking session management
    void startHacking(int securityLevel);
    void endHacking();

    // Hacking actions
    bool injectCode(const std::string& code);   // Push action
    bool bypassFirewall();
    bool decryptLayer();
    bool undoLastAction();      // Pop from stack

    // Status
    bool isHacking() const;
    bool isSuccess() const;
    bool isFailed() const;
    int getProgress() const;
    int getRemainingAttempts() const;

    // Display
    void displayHackingInterface() const;
    void displayActionHistory() const;

    // Rewards
    Item* getHackReward() const;
};

/*******************************************************************************
 * Main Game Class
 *
 * Central coordinator for all game systems.
 ******************************************************************************/
class Game {
private:
    // Core game objects
    Player* player;
    Inventory* inventory;
    AbilityTree* abilities;
    GridMap* worldMap;
    EventLog* eventLog;
    ItemDatabase* itemDatabase;
    Overseer* overseer;

    // Subsystems
    CombatSystem* combatSystem;
    HackingSystem* hackingSystem;

    // Game state
    GameState currentState;
    GameState previousState;
    int turnCount;
    bool isRunning;

    // Statistics
    int enemiesDefeated;
    int sectorsExplored;
    int hacksCompleted;

    // Private helper methods
    void initializeSystems();
    void cleanupSystems();
    void processInput(const std::string& input);

    // State handlers
    void handleMainMenu();
    void handleNewGame();
    void handlePlaying();
    void handleCombat();
    void handleHacking();
    void handleInventory();
    void handleAbilities();
    void handleEventLog();
    void handleSaveMenu();
    void handleOptions();
    void handleGameOver();
    void handleVictory();

    // Utility
    void clearScreen() const;
    std::string getInput() const;
    void waitForInput() const;

public:
    // Constructor
    Game();

    // Destructor
    ~Game();

    // Main game control
    void run();                 // Main game loop
    void quit();

    // State management
    void setState(GameState state);
    GameState getState() const;

    // Game actions
    void newGame(const std::string& playerName);
    bool saveGame(const std::string& slot);
    bool loadGame(const std::string& slot);

    // Exploration
    void explore(const std::string& direction);
    void scanArea();
    void interactWithTerminal();
    void rest();

    // Display methods
    void displayTitle() const;
    void displayMainMenu() const;
    void displayGameScreen() const;
    void displayStats() const;

    // Getters
    Player* getPlayer() const;
    Inventory* getInventory() const;
    AbilityTree* getAbilities() const;
    int getTurnCount() const;

    // Events
    void logEvent(const std::string& description, const std::string& type);
};

/*******************************************************************************
 * Utility Functions
 ******************************************************************************/
void displayBanner();
void displayGameOver();
void displayVictory();
int getRandomNumber(int min, int max);
std::string formatHealth(int current, int max);

#endif // GAME_H