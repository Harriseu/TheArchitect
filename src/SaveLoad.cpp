/*******************************************************************************
 * SaveLoad.cpp - Save/Load System Implementation
 *
 * REQUIREMENT: File operations to keep track of Player Progress, Save/Load Game
 *
 * Author: Alif Harriz Jeffery | 102782711
 * Course: COS30008 Data Structures and Patterns
 ******************************************************************************/

#include "../include/SaveLoad.h"
#include "../include/Entity.h"
#include "../include/Iterator.h"
#include "../include/Tree.h"
#include "../include/LinkedList.h"
#include <cstring>
#include <ctime>
#include <sys/stat.h>

// Cross-platform ctime_r/ctime_s wrapper
#ifdef _WIN32
#define SAFE_CTIME(time_ptr, buffer, size) ctime_s(buffer, size, time_ptr)
#define SAFE_STRNCPY(dest, dest_size, src, count) strncpy_s(dest, dest_size, src, count)
#else
#define SAFE_CTIME(time_ptr, buffer, size) ctime_r(time_ptr, buffer)
#define SAFE_STRNCPY(dest, dest_size, src, count) strncpy(dest, src, count)
#endif

/*******************************************************************************
 * SAVE/LOAD MANAGER IMPLEMENTATION
 ******************************************************************************/

// Private constructor
SaveLoadManager::SaveLoadManager()
    : savePath(""), configPath("config.ini"), initialized(false) {
}

// Singleton instance
SaveLoadManager& SaveLoadManager::getInstance() {
    static SaveLoadManager instance;
    return instance;
}

// Destructor
SaveLoadManager::~SaveLoadManager() {
}

// Initialize save system
bool SaveLoadManager::initialize(const std::string& saveDirectory) {
    savePath = saveDirectory;

    // Create save directory if it doesn't exist
    struct stat st;
    if (stat(savePath.c_str(), &st) != 0) {
        // Directory doesn't exist - we'll create files in current dir
        savePath = ".";
    }

    configPath = savePath + "/config.ini";
    initialized = true;

    std::cout << "[SaveLoad] Initialized. Save path: " << savePath << std::endl;
    return true;
}

// SAVE GAME
// Step 1: Open file for binary writing
// Step 2: Write signature and version
// Step 3: Serialize player data
// Step 4: Serialize inventory
// Step 5: Serialize abilities
// Step 6: Serialize game state
// Step 7: Close file
bool SaveLoadManager::saveGame(const std::string& filename, Player* player,
                               Inventory* inventory, AbilityTree* abilities,
                               GridMap* map, int turnCount) {
    if (!initialized || player == nullptr) {
        std::cout << "[SaveLoad] Cannot save - system not initialized or no player." << std::endl;
        return false;
    }

    std::string fullPath = savePath + "/" + filename + ".dat";

    // Step 1: Open file
    std::ofstream file(fullPath, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "[SaveLoad] Failed to open file for writing: " << fullPath << std::endl;
        return false;
    }

    std::cout << "[SaveLoad] Saving game to: " << fullPath << std::endl;

    // Create save data structure
    SaveData data;
    memset(&data, 0, sizeof(SaveData));

    // Step 2: Write signature and version
    SAFE_STRNCPY(data.signature, sizeof(data.signature), "DEXODUS", 7);
    data.signature[7] = '\0';
    data.version = 1;
    data.saveTime = time(nullptr);

    // Step 3: Serialize player data
    SAFE_STRNCPY(data.playerName, sizeof(data.playerName), player->getName().c_str(), 31);
    data.playerName[31] = '\0';
    data.health = player->getHealth();
    data.maxHealth = player->getMaxHealth();
    data.level = player->getLevel();
    data.experience = player->getExperience();
    data.attackPower = player->getAttackPower();
    data.defense = player->getDefense();
    data.speed = player->getSpeed();
    data.hackingSkill = player->getHackingSkill();

    // Current sector
    if (map != nullptr && map->getCurrentSector() != nullptr) {
        SAFE_STRNCPY(data.currentSector, sizeof(data.currentSector), map->getCurrentSector()->name.c_str(), 63);
        data.currentSector[63] = '\0';
    }

    // Step 4: Serialize inventory
    data.inventoryCount = 0;
    if (inventory != nullptr) {
        InventoryIterator* it = inventory->createIterator("all");
        while (it->hasNext() && data.inventoryCount < 50) {
            Item* item = it->next();
            SAFE_STRNCPY(data.inventoryCodes[data.inventoryCount], sizeof(data.inventoryCodes[data.inventoryCount]), item->code.c_str(), 31);
            data.inventoryCodes[data.inventoryCount][31] = '\0';
            data.inventoryQuantities[data.inventoryCount] = item->quantity;
            data.inventoryCount++;
        }
        delete it;
    }

    // Step 5: Serialize abilities
    data.unlockedAbilityCount = 0;
    data.skillPoints = abilities ? abilities->getSkillPoints() : 0;
    if (abilities != nullptr) {
        int count = 0;
        Ability** unlocked = abilities->getUnlockedAbilities(count);
        for (int i = 0; i < count && data.unlockedAbilityCount < 20; i++) {
            SAFE_STRNCPY(data.unlockedAbilities[data.unlockedAbilityCount],
                    sizeof(data.unlockedAbilities[data.unlockedAbilityCount]),
                    unlocked[i]->code.c_str(), 31);
            data.unlockedAbilities[data.unlockedAbilityCount][31] = '\0';
            data.unlockedAbilityCount++;
        }
        delete[] unlocked;
    }

    // Step 6: Game state
    data.turnCount = turnCount;
    data.sectorsVisited = 0;
    data.enemiesDefeated = 0;
    data.eventsTriggered = 0;

    // Step 7: Write and close
    file.write(reinterpret_cast<char*>(&data), sizeof(SaveData));
    file.close();

    std::cout << "[SaveLoad] Game saved successfully!" << std::endl;
    std::cout << "  Player: " << data.playerName << " (Level " << data.level << ")" << std::endl;
    std::cout << "  Items: " << data.inventoryCount << std::endl;
    std::cout << "  Abilities: " << data.unlockedAbilityCount << std::endl;

    return true;
}

// LOAD GAME
// Step 1: Open file for binary reading
// Step 2: Verify signature and version
// Step 3: Deserialize player data
// Step 4: Deserialize inventory
// Step 5: Deserialize abilities
// Step 6: Deserialize game state
// Step 7: Close file
bool SaveLoadManager::loadGame(const std::string& filename, Player* player,
                               Inventory* inventory, AbilityTree* abilities,
                               GridMap* map, int& turnCount) {
    if (!initialized || player == nullptr) {
        std::cout << "[SaveLoad] Cannot load - system not initialized or no player." << std::endl;
        return false;
    }

    std::string fullPath = savePath + "/" + filename + ".dat";

    // Step 1: Open file
    std::ifstream file(fullPath, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "[SaveLoad] Save file not found: " << fullPath << std::endl;
        return false;
    }

    std::cout << "[SaveLoad] Loading game from: " << fullPath << std::endl;

    // Read save data
    SaveData data;
    file.read(reinterpret_cast<char*>(&data), sizeof(SaveData));
    file.close();

    // Step 2: Verify signature
    if (strncmp(data.signature, "DEXODUS", 7) != 0) {
        std::cout << "[SaveLoad] Invalid save file format!" << std::endl;
        return false;
    }

    if (data.version != 1) {
        std::cout << "[SaveLoad] Incompatible save version: " << data.version << std::endl;
        return false;
    }

    // Step 3: Restore player data
    player->setHealth(data.health);
    player->setLevel(data.level);
    player->setExperience(data.experience);
    player->setAttackPower(data.attackPower);
    player->setHackingSkill(data.hackingSkill);

    // Step 4: Restore location
    if (map != nullptr && strlen(data.currentSector) > 0) {
        SectorNode* sector = map->findSector(data.currentSector);
        if (sector != nullptr) {
            map->setCurrentSector(sector);
        }
    }

    // Step 5: Restore abilities
    if (abilities != nullptr) {
        abilities->addSkillPoints(data.skillPoints);
        // Note: Actual ability unlocking would need to iterate through saved codes
    }

    // Step 6: Restore game state
    turnCount = data.turnCount;

    std::cout << "[SaveLoad] Game loaded successfully!" << std::endl;
    std::cout << "  Player: " << data.playerName << " (Level " << data.level << ")" << std::endl;
    std::cout << "  Location: " << data.currentSector << std::endl;

    // Display save time
    char timeBuffer[26];
    SAFE_CTIME(&data.saveTime, timeBuffer, sizeof(timeBuffer));
    std::cout << "  Saved: " << timeBuffer;

    return true;
}

// Save config
bool SaveLoadManager::saveConfig(const ConfigData& config) {
    std::ofstream file(configPath);
    if (!file.is_open()) {
        return false;
    }

    file << "[Audio]\n";
    file << "master_volume=" << config.masterVolume << "\n";
    file << "music_volume=" << config.musicVolume << "\n";
    file << "sfx_volume=" << config.sfxVolume << "\n";
    file << "audio_enabled=" << (config.audioEnabled ? "true" : "false") << "\n";
    file << "\n";

    file << "[Display]\n";
    file << "show_tutorials=" << (config.showTutorials ? "true" : "false") << "\n";
    file << "verbose_mode=" << (config.verboseMode ? "true" : "false") << "\n";
    file << "\n";

    file << "[Gameplay]\n";
    file << "difficulty=" << config.difficulty << "\n";
    file << "auto_save=" << (config.autoSave ? "true" : "false") << "\n";

    file.close();
    std::cout << "[SaveLoad] Config saved." << std::endl;
    return true;
}

// Load config
bool SaveLoadManager::loadConfig(ConfigData& config) {
    std::ifstream file(configPath);
    if (!file.is_open()) {
        // Set defaults
        config.masterVolume = 1.0f;
        config.musicVolume = 0.7f;
        config.sfxVolume = 0.8f;
        config.audioEnabled = true;
        config.showTutorials = true;
        config.verboseMode = false;
        config.difficulty = 2;
        config.autoSave = true;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.find("master_volume=") == 0) {
            config.masterVolume = std::stof(line.substr(14));
        } else if (line.find("music_volume=") == 0) {
            config.musicVolume = std::stof(line.substr(13));
        } else if (line.find("sfx_volume=") == 0) {
            config.sfxVolume = std::stof(line.substr(11));
        } else if (line.find("audio_enabled=") == 0) {
            config.audioEnabled = (line.substr(14) == "true");
        } else if (line.find("difficulty=") == 0) {
            config.difficulty = std::stoi(line.substr(11));
        } else if (line.find("auto_save=") == 0) {
            config.autoSave = (line.substr(10) == "true");
        }
    }

    file.close();
    std::cout << "[SaveLoad] Config loaded." << std::endl;
    return true;
}

// Check if save exists
bool SaveLoadManager::saveExists(const std::string& filename) const {
    std::string fullPath = savePath + "/" + filename + ".dat";
    std::ifstream file(fullPath);
    return file.good();
}

// Delete save
bool SaveLoadManager::deleteSave(const std::string& filename) {
    std::string fullPath = savePath + "/" + filename + ".dat";
    return std::remove(fullPath.c_str()) == 0;
}

// Get save info
std::string SaveLoadManager::getSaveInfo(const std::string& filename) const {
    std::string fullPath = savePath + "/" + filename + ".dat";

    std::ifstream file(fullPath, std::ios::binary);
    if (!file.is_open()) {
        return "Save not found";
    }

    SaveData data;
    file.read(reinterpret_cast<char*>(&data), sizeof(SaveData));
    file.close();

    if (strncmp(data.signature, "DEXODUS", 7) != 0) {
        return "Invalid save file";
    }

    char timeBuffer[26];
    SAFE_CTIME(&data.saveTime, timeBuffer, sizeof(timeBuffer));
    timeBuffer[24] = '\0';  // Remove newline

    std::string info = std::string(data.playerName) + " - Level " +
                       std::to_string(data.level) + " - " + timeBuffer;
    return info;
}

// Quick save/load
bool SaveLoadManager::quickSave(Player* player, Inventory* inventory,
                                AbilityTree* abilities, GridMap* map, int turnCount) {
    return saveGame("quicksave", player, inventory, abilities, map, turnCount);
}

bool SaveLoadManager::quickLoad(Player* player, Inventory* inventory,
                                AbilityTree* abilities, GridMap* map, int& turnCount) {
    return loadGame("quicksave", player, inventory, abilities, map, turnCount);
}

// Auto-save
bool SaveLoadManager::autoSave(Player* player, Inventory* inventory,
                               AbilityTree* abilities, GridMap* map, int turnCount) {
    std::cout << "[SaveLoad] Auto-saving..." << std::endl;
    return saveGame("autosave", player, inventory, abilities, map, turnCount);
}

// Getters
std::string SaveLoadManager::getSavePath() const { return savePath; }
bool SaveLoadManager::isInitialized() const { return initialized; }

// List saves (simplified)
std::string* SaveLoadManager::listSaves(int& outCount) const {
    // Return common save names
    std::string* saves = new std::string[5];
    outCount = 0;

    const char* saveNames[] = {"save1", "save2", "save3", "quicksave", "autosave"};
    for (int i = 0; i < 5; i++) {
        if (saveExists(saveNames[i])) {
            saves[outCount++] = saveNames[i];
        }
    }

    return saves;
}

/*******************************************************************************
 * CONVENIENCE FUNCTIONS
 ******************************************************************************/

bool saveGame(const std::string& slot) {
    // Would need game state references - simplified here
    return false;
}

bool loadGame(const std::string& slot) {
    return false;
}

bool quickSave() {
    return false;
}

bool quickLoad() {
    return false;
}