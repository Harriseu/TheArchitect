/*******************************************************************************
 * SaveLoad.h - File Operations for Save/Load System
 *
 * REQUIREMENT: File operations to keep track of Player Progress, Save/Load Game
 *
 * This module handles all file I/O for the game:
 * - Saving game state to file
 * - Loading game state from file
 * - Config file management
 * - Auto-save functionality
 *
 * Save File Format: Binary (.dat) for efficiency
 * Config File Format: Text (.ini) for user editing
 *
 * Author: Digital Exodus Project
 * Course: COS30008 Data Structures and Patterns
 ******************************************************************************/

#ifndef SAVELOAD_H
#define SAVELOAD_H

#include <string>
#include <fstream>
#include <iostream>
#include <ctime>

// Forward declarations
class Player;
class Inventory;
class AbilityTree;
class GridMap;
class EventLog;

/*******************************************************************************
 * Save Data Structure
 *
 * Contains all data needed to restore game state.
 ******************************************************************************/
struct SaveData {
    // Header
    char signature[8];          // "DEXODUS" + version
    int version;                // Save format version
    time_t saveTime;            // When saved

    // Player data
    char playerName[32];
    int health;
    int maxHealth;
    int level;
    int experience;
    int attackPower;
    int defense;
    int speed;
    int hackingSkill;
    int skillPoints;

    // Location
    char currentSector[64];

    // Inventory (simplified - stores item codes)
    int inventoryCount;
    char inventoryCodes[50][32];
    int inventoryQuantities[50];

    // Abilities (stores unlocked ability codes)
    int unlockedAbilityCount;
    char unlockedAbilities[20][32];

    // Game state
    int turnCount;
    int sectorsVisited;
    int enemiesDefeated;
    int eventsTriggered;
};

/*******************************************************************************
 * Config Data Structure
 *
 * Contains game settings.
 ******************************************************************************/
struct ConfigData {
    // Audio settings
    float masterVolume;
    float musicVolume;
    float sfxVolume;
    bool audioEnabled;

    // Display settings
    bool showTutorials;
    bool verboseMode;

    // Gameplay settings
    int difficulty;             // 1=Easy, 2=Normal, 3=Hard
    bool autoSave;
};

/*******************************************************************************
 * Save/Load Manager
 *
 * Handles all file operations for game persistence.
 *
 * Operations:
 * - saveGame(): Write current state to file
 * - loadGame(): Read state from file
 * - saveConfig(): Write settings to config
 * - loadConfig(): Read settings from config
 * - listSaves(): Get available save files
 ******************************************************************************/
class SaveLoadManager {
private:
    std::string savePath;       // Directory for save files
    std::string configPath;     // Path to config file
    bool initialized;

    // Helper functions
    bool writePlayerData(std::ofstream& file, Player* player);
    bool readPlayerData(std::ifstream& file, Player* player);
    bool writeInventoryData(std::ofstream& file, Inventory* inventory);
    bool readInventoryData(std::ifstream& file, Inventory* inventory);
    bool writeAbilityData(std::ofstream& file, AbilityTree* abilities);
    bool readAbilityData(std::ifstream& file, AbilityTree* abilities);

    // Private constructor (Singleton)
    SaveLoadManager();

public:
    // Singleton access
    static SaveLoadManager& getInstance();

    // Destructor
    ~SaveLoadManager();

    // Prevent copying
    SaveLoadManager(const SaveLoadManager&) = delete;
    SaveLoadManager& operator=(const SaveLoadManager&) = delete;

    // Initialization
    bool initialize(const std::string& saveDirectory);

    // Save Operations
    // Step 1: Open file for binary writing
    // Step 2: Write signature and version
    // Step 3: Serialize player data
    // Step 4: Serialize inventory
    // Step 5: Serialize abilities
    // Step 6: Serialize game state
    // Step 7: Close file
    bool saveGame(const std::string& filename, Player* player,
                  Inventory* inventory, AbilityTree* abilities,
                  GridMap* map, int turnCount);

    // Load Operations
    // Step 1: Open file for binary reading
    // Step 2: Verify signature and version
    // Step 3: Deserialize player data
    // Step 4: Deserialize inventory
    // Step 5: Deserialize abilities
    // Step 6: Deserialize game state
    // Step 7: Close file
    bool loadGame(const std::string& filename, Player* player,
                  Inventory* inventory, AbilityTree* abilities,
                  GridMap* map, int& turnCount);

    // Config Operations
    bool saveConfig(const ConfigData& config);
    bool loadConfig(ConfigData& config);

    // Utility
    bool saveExists(const std::string& filename) const;
    bool deleteSave(const std::string& filename);
    std::string* listSaves(int& outCount) const;
    std::string getSaveInfo(const std::string& filename) const;

    // Quick save/load
    bool quickSave(Player* player, Inventory* inventory,
                   AbilityTree* abilities, GridMap* map, int turnCount);
    bool quickLoad(Player* player, Inventory* inventory,
                   AbilityTree* abilities, GridMap* map, int& turnCount);

    // Auto-save
    bool autoSave(Player* player, Inventory* inventory,
                  AbilityTree* abilities, GridMap* map, int turnCount);

    // Getters
    std::string getSavePath() const;
    bool isInitialized() const;
};

/*******************************************************************************
 * Convenience functions for global access
 ******************************************************************************/
bool saveGame(const std::string& slot);
bool loadGame(const std::string& slot);
bool quickSave();
bool quickLoad();

#endif // SAVELOAD_H
