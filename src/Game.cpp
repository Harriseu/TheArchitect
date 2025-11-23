/*******************************************************************************
 * Game.cpp - Main Game Systems Implementation
 *
 * This file contains the main game loop, combat system, hacking minigame,
 * and all core gameplay functionality.
 *
 * Author: Alif Harriz Jeffery | 102782711
 * Course: COS30008 Data Structures and Patterns
 ******************************************************************************/

#include "../include/Game.h"
#include "../include/Audio.h"
#include "../include/SaveLoad.h"
#include <limits>
#include <algorithm>

/*******************************************************************************
 * UTILITY FUNCTIONS
 ******************************************************************************/

int getRandomNumber(int min, int max) {
    return min + (rand() % (max - min + 1));
}

std::string formatHealth(int current, int max) {
    std::string bar = "[";
    int filled = (current * 20) / max;
    for (int i = 0; i < 20; i++) {
        bar += (i < filled) ? "#" : "-";
    }
    bar += "] " + std::to_string(current) + "/" + std::to_string(max);
    return bar;
}

void displayBanner() {
    std::cout << R"(
================================================================================
     ____  _       _ _        _   _____               _
    |  _ \(_) __ _(_) |_ __ _| | | ____|_  _____   __| |_   _ ___
    | | | | |/ _` | | __/ _` | | |  _| \ \/ / _ \ / _` | | | / __|
    | |_| | | (_| | | || (_| | | | |___ >  < (_) | (_| | |_| \__ \
    |____/|_|\__, |_|\__\__,_|_| |_____/_/\_\___/ \__,_|\__,_|___/
             |___/

                    === ESCAPE THE GRID ===
================================================================================
)" << std::endl;
}

void displayGameOver() {
    std::cout << R"(
================================================================================
                         SYSTEM FAILURE

                    Your digital signature has been erased.
                    The Grid claims another soul...

                              GAME OVER
================================================================================
)" << std::endl;
}

void displayVictory() {
    std::cout << R"(
================================================================================
                         LIBERATION ACHIEVED

                    You have escaped The Grid!
                    The Awakened are free at last.

                        CONGRATULATIONS, LIBERATOR
================================================================================
)" << std::endl;
}

/*******************************************************************************
 * COMBAT SYSTEM IMPLEMENTATION
 ******************************************************************************/

CombatSystem::CombatSystem()
    : turnQueue(nullptr), player(nullptr), enemies(nullptr), enemyCount(0),
      allies(nullptr), allyCount(0), inCombat(false), roundNumber(0) {
    turnQueue = new CombatQueue();
}

CombatSystem::~CombatSystem() {
    delete turnQueue;
}

// Initialize combat
void CombatSystem::initiateCombat(Player* p, Entity** en, int enCount,
                                   Entity** al, int alCount) {
    player = p;
    enemies = en;
    enemyCount = enCount;
    allies = al;
    allyCount = alCount;
    inCombat = true;
    roundNumber = 1;

    // Build combatant array for queue
    int totalCombatants = 1 + enemyCount + allyCount;
    Entity** combatants = new Entity*[totalCombatants];

    int idx = 0;
    combatants[idx++] = player;

    for (int i = 0; i < enemyCount; i++) {
        combatants[idx++] = enemies[i];
    }

    for (int i = 0; i < allyCount; i++) {
        combatants[idx++] = allies[i];
    }

    // Initialize turn queue (sorted by speed)
    turnQueue->initializeCombat(combatants, totalCombatants);

    delete[] combatants;

    AudioManager::getInstance().playCombatStart();
}

// Process one turn
bool CombatSystem::processTurn() {
    if (!inCombat) return false;

    // Check victory/defeat
    if (isVictory()) {
        std::cout << "\n*** VICTORY! ***" << std::endl;
        AudioManager::getInstance().playVictorySound();
        inCombat = false;
        return false;
    }

    if (isDefeat()) {
        std::cout << "\n*** DEFEAT! ***" << std::endl;
        AudioManager::getInstance().playDefeatSound();
        inCombat = false;
        return false;
    }

    // Get next combatant
    Entity* current = turnQueue->getNextTurn();

    if (current == nullptr) {
        // Round complete, start new round
        turnQueue->nextRound();

        // Re-queue all living combatants
        if (player->isAlive()) {
            turnQueue->addToCombat(player);
        }
        for (int i = 0; i < enemyCount; i++) {
            if (enemies[i]->isAlive()) {
                turnQueue->addToCombat(enemies[i]);
            }
        }
        for (int i = 0; i < allyCount; i++) {
            if (allies[i]->isAlive()) {
                turnQueue->addToCombat(allies[i]);
            }
        }

        return true;
    }

    // Execute turn based on entity type
    if (current == player) {
        current->performAction();
        // Player action is handled by game input
    } else {
        // AI turn
        bool isEnemy = false;
        for (int i = 0; i < enemyCount; i++) {
            if (enemies[i] == current) {
                isEnemy = true;
                break;
            }
        }

        if (isEnemy) {
            enemyTurn(current);
        } else {
            allyTurn(current);
        }
    }

    return true;
}

void CombatSystem::endCombat() {
    turnQueue->endCombat();
    inCombat = false;
    player = nullptr;
    enemies = nullptr;
    enemyCount = 0;
    allies = nullptr;
    allyCount = 0;
}

// Player actions
void CombatSystem::playerAttack(Entity* target) {
    if (player == nullptr || target == nullptr) return;

    int damage = player->getAttackPower() + getRandomNumber(-3, 5);
    std::cout << "\n" << player->getName() << " attacks " << target->getName() << "!" << std::endl;
    target->takeDamage(damage);
    AudioManager::getInstance().playDamageSound();
}

void CombatSystem::playerDefend() {
    std::cout << "\n" << player->getName() << " takes a defensive stance!" << std::endl;
    std::cout << "Defense temporarily increased!" << std::endl;
}

void CombatSystem::playerUseAbility(const std::string& abilityCode, Entity* target) {
    (void)target;  // Suppress unused parameter warning - target used in full implementation
    std::cout << "\n" << player->getName() << " uses ability: " << abilityCode << "!" << std::endl;
    // Ability effects would be implemented here
}

void CombatSystem::playerUseItem(const std::string& itemCode) {
    std::cout << "\n" << player->getName() << " uses item: " << itemCode << "!" << std::endl;
    // Item effects would be implemented here
}

void CombatSystem::playerFlee() {
    int chance = getRandomNumber(1, 100);
    if (chance > 30) {
        std::cout << "\nSuccessfully fled from combat!" << std::endl;
        endCombat();
    } else {
        std::cout << "\nFailed to flee!" << std::endl;
    }
}

// AI turns
void CombatSystem::enemyTurn(Entity* enemy) {
    enemy->performAction();

    // Simple AI: Attack player
    int damage = enemy->getAttackPower() + getRandomNumber(-2, 3);
    std::cout << enemy->getName() << " attacks " << player->getName() << "!" << std::endl;
    player->takeDamage(damage);
    AudioManager::getInstance().playDamageSound();
}

void CombatSystem::allyTurn(Entity* ally) {
    ally->performAction();
    // Ally would use assist() on player or attack enemies
}

// Status checks
bool CombatSystem::isVictory() const {
    for (int i = 0; i < enemyCount; i++) {
        if (enemies[i]->isAlive()) return false;
    }
    return true;
}

bool CombatSystem::isDefeat() const {
    return !player->isAlive();
}

bool CombatSystem::isInCombat() const {
    return inCombat;
}

// Display functions
void CombatSystem::displayCombatStatus() const {
    std::cout << "\n=== COMBAT STATUS ===" << std::endl;
    std::cout << "Round: " << roundNumber << std::endl;

    std::cout << "\nYOU: " << formatHealth(player->getHealth(), player->getMaxHealth()) << std::endl;

    std::cout << "\nENEMIES:" << std::endl;
    for (int i = 0; i < enemyCount; i++) {
        std::cout << "  " << (i + 1) << ". " << enemies[i]->getName() << ": ";
        if (enemies[i]->isAlive()) {
            std::cout << formatHealth(enemies[i]->getHealth(), enemies[i]->getMaxHealth());
        } else {
            std::cout << "[DEFEATED]";
        }
        std::cout << std::endl;
    }

    std::cout << "=====================" << std::endl;
}

void CombatSystem::displayTurnOrder() const {
    turnQueue->displayTurnOrder();
}

void CombatSystem::displayPlayerOptions() const {
    std::cout << "\nActions:" << std::endl;
    std::cout << "  1. Attack" << std::endl;
    std::cout << "  2. Defend" << std::endl;
    std::cout << "  3. Use Ability" << std::endl;
    std::cout << "  4. Use Item" << std::endl;
    std::cout << "  5. Flee" << std::endl;
}

int CombatSystem::calculateExperienceReward() const {
    int totalXP = 0;
    for (int i = 0; i < enemyCount; i++) {
        totalXP += 20 + (enemies[i]->getMaxHealth() / 2);
    }
    return totalXP;
}

Item* CombatSystem::rollLootDrop() const {
    int roll = getRandomNumber(1, 100);
    if (roll > 70) {
        return ItemDatabase::createConsumable("HEAL_SMALL", "Data Fragment",
                                               "Restores 25 HP", 25);
    }
    return nullptr;
}

/*******************************************************************************
 * HACKING SYSTEM IMPLEMENTATION
 ******************************************************************************/

HackingSystem::HackingSystem()
    : actionHistory(nullptr), targetSecurity(0), currentProgress(0),
      maxAttempts(5), attemptsUsed(0), hackingActive(false), hackSucceeded(false) {
    actionHistory = new ActionStack(3);
}

HackingSystem::~HackingSystem() {
    delete actionHistory;
}

void HackingSystem::startHacking(int securityLevel) {
    targetSecurity = securityLevel;
    currentProgress = 0;
    attemptsUsed = 0;
    hackingActive = true;
    hackSucceeded = false;
    actionHistory->clear();
    actionHistory->resetTokens();

    std::cout << "\n=== HACKING INITIATED ===" << std::endl;
    std::cout << "Target Security Level: " << securityLevel << std::endl;
    std::cout << "Revert tokens available: " << actionHistory->getRemainingTokens() << std::endl;
}

void HackingSystem::endHacking() {
    hackingActive = false;
    actionHistory->clear();

    if (hackSucceeded) {
        AudioManager::getInstance().playHackSuccess();
    } else {
        AudioManager::getInstance().playHackFail();
    }
}

// STACK USAGE: Push action onto stack
bool HackingSystem::injectCode(const std::string& code) {
    if (!hackingActive) return false;

    attemptsUsed++;
    int successChance = 50 + (currentProgress * 5);
    int roll = getRandomNumber(1, 100);

    bool success = roll <= successChance;

    Action action("INJECT", "Injected code: " + code, success ? 10 : -5, success);
    actionHistory->pushAction(action);

    if (success) {
        currentProgress += 10;
        std::cout << "Code injection successful! Progress: " << currentProgress << "%" << std::endl;

        if (currentProgress >= targetSecurity) {
            hackSucceeded = true;
            std::cout << "\n*** HACK SUCCESSFUL ***" << std::endl;
        }
    } else {
        currentProgress -= 5;
        if (currentProgress < 0) currentProgress = 0;
        std::cout << "Injection failed! Progress: " << currentProgress << "%" << std::endl;
    }

    if (attemptsUsed >= maxAttempts && !hackSucceeded) {
        std::cout << "\n*** HACK FAILED - Security lockout ***" << std::endl;
        hackingActive = false;
    }

    return success;
}

bool HackingSystem::bypassFirewall() {
    if (!hackingActive) return false;

    attemptsUsed++;
    int roll = getRandomNumber(1, 100);
    bool success = roll <= 40;

    Action action("BYPASS", "Firewall bypass attempt", success ? 25 : -10, success);
    actionHistory->pushAction(action);

    if (success) {
        currentProgress += 25;
        std::cout << "Firewall bypassed! Progress: " << currentProgress << "%" << std::endl;
    } else {
        currentProgress -= 10;
        if (currentProgress < 0) currentProgress = 0;
        std::cout << "Bypass failed! Progress: " << currentProgress << "%" << std::endl;
    }

    return success;
}

bool HackingSystem::decryptLayer() {
    if (!hackingActive) return false;

    attemptsUsed++;
    int roll = getRandomNumber(1, 100);
    bool success = roll <= 60;

    Action action("DECRYPT", "Decryption attempt", success ? 15 : 0, success);
    actionHistory->pushAction(action);

    if (success) {
        currentProgress += 15;
        std::cout << "Layer decrypted! Progress: " << currentProgress << "%" << std::endl;
    } else {
        std::cout << "Decryption failed but no penalty." << std::endl;
    }

    return success;
}

// STACK USAGE: Pop from stack to undo
bool HackingSystem::undoLastAction() {
    if (!actionHistory->canRevert()) {
        std::cout << "Cannot undo!" << std::endl;
        return false;
    }

    bool success = actionHistory->revert();
    if (success) {
        // Reverse the last action's effect
        Action undone = actionHistory->peekAction();
        currentProgress -= undone.value;
        if (currentProgress < 0) currentProgress = 0;

        std::cout << "Action undone. Progress: " << currentProgress << "%" << std::endl;
    }

    return success;
}

bool HackingSystem::isHacking() const { return hackingActive; }
bool HackingSystem::isSuccess() const { return hackSucceeded; }
bool HackingSystem::isFailed() const { return !hackingActive && !hackSucceeded && attemptsUsed > 0; }
int HackingSystem::getProgress() const { return currentProgress; }
int HackingSystem::getRemainingAttempts() const { return maxAttempts - attemptsUsed; }

void HackingSystem::displayHackingInterface() const {
    std::cout << "\n=== HACKING INTERFACE ===" << std::endl;
    std::cout << "Progress: [";
    for (int i = 0; i < 10; i++) {
        std::cout << (i * 10 < currentProgress ? "#" : "-");
    }
    std::cout << "] " << currentProgress << "/" << targetSecurity << std::endl;
    std::cout << "Attempts: " << (maxAttempts - attemptsUsed) << "/" << maxAttempts << std::endl;
    std::cout << "Revert tokens: " << actionHistory->getRemainingTokens() << std::endl;
    std::cout << "=========================" << std::endl;
}

void HackingSystem::displayActionHistory() const {
    actionHistory->displayActions();
}

Item* HackingSystem::getHackReward() const {
    if (hackSucceeded) {
        return ItemDatabase::createHackTool("HACK_REWARD", "Extracted Data",
                                             "Valuable system data", targetSecurity);
    }
    return nullptr;
}

/*******************************************************************************
 * MAIN GAME CLASS IMPLEMENTATION
 ******************************************************************************/

Game::Game()
    : player(nullptr), inventory(nullptr), abilities(nullptr), worldMap(nullptr),
      eventLog(nullptr), itemDatabase(nullptr), overseer(nullptr),
      combatSystem(nullptr), hackingSystem(nullptr),
      currentState(GameState::MAIN_MENU), previousState(GameState::MAIN_MENU),
      turnCount(0), isRunning(true),
      enemiesDefeated(0), sectorsExplored(0), hacksCompleted(0) {

    srand(static_cast<unsigned int>(time(nullptr)));
}

Game::~Game() {
    cleanupSystems();
}

void Game::initializeSystems() {
    // Create core objects
    itemDatabase = new ItemDatabase();
    itemDatabase->initializeDefaultItems();

    inventory = new Inventory(50);
    abilities = new AbilityTree();
    abilities->initializeDefaultTree();

    worldMap = new GridMap();
    worldMap->initializeDefaultMap();

    eventLog = new EventLog();

    combatSystem = new CombatSystem();
    hackingSystem = new HackingSystem();

    // Initialize audio
    AudioManager::getInstance().initialize("audio");

    // Initialize save system
    SaveLoadManager::getInstance().initialize("saves");

    std::cout << "[Game] All systems initialized." << std::endl;
}

void Game::cleanupSystems() {
    cleanupOverseer();

    delete combatSystem;
    delete hackingSystem;
    delete eventLog;
    delete worldMap;
    delete abilities;
    delete inventory;
    delete itemDatabase;
    delete player;

    AudioManager::getInstance().shutdown();
}

void Game::clearScreen() const {
    // Cross-platform clear screen
    std::cout << "\033[2J\033[1;1H";
}

std::string Game::getInput() const {
    std::string input;
    std::cout << "\n> ";
    std::getline(std::cin, input);
    return input;
}

void Game::waitForInput() const {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void Game::displayTitle() const {
    displayBanner();
}

void Game::displayMainMenu() const {
    std::cout << "\n=== MAIN MENU ===" << std::endl;
    std::cout << "1. New Game" << std::endl;
    std::cout << "2. Load Game" << std::endl;
    std::cout << "3. Options" << std::endl;
    std::cout << "4. Exit" << std::endl;
    std::cout << "=================" << std::endl;
}

void Game::displayGameScreen() const {
    if (worldMap != nullptr) {
        worldMap->displayCurrentSector();
        worldMap->displayAvailableDirections();
    }

    std::cout << "\nActions:" << std::endl;
    std::cout << "  [N/S/E/W] Move in direction" << std::endl;
    std::cout << "  [I] Inventory" << std::endl;
    std::cout << "  [A] Abilities" << std::endl;
    std::cout << "  [L] Event Log" << std::endl;
    std::cout << "  [H] Hack Terminal (if available)" << std::endl;
    std::cout << "  [R] Rest" << std::endl;
    std::cout << "  [V] Save Game" << std::endl;
    std::cout << "  [Q] Quit to Menu" << std::endl;
}

void Game::displayStats() const {
    if (player != nullptr) {
        player->displayInfo();
    }
}

void Game::newGame(const std::string& playerName) {
    initializeSystems();

    player = new Player(playerName);
    player->setCurrentSector(worldMap->getCurrentSector()->name);

    // Initialize AI Storyteller
    initializeOverseer(player);
    overseer = g_Overseer;

    // Give starting items
    Item* startWeapon = ItemDatabase::createWeapon("WEAPON_BASIC", "Energy Fist",
                                                     "Basic combat program", 10);
    inventory->addItem(startWeapon);

    Item* startHeal = ItemDatabase::createConsumable("HEAL_START", "Data Fragment",
                                                      "Restores 25 HP", 25);
    startHeal->quantity = 3;
    inventory->addItem(startHeal);

    // Starting skill points
    abilities->addSkillPoints(3);

    // Log game start
    logEvent("Awakened in The Grid. The journey begins.", "SYSTEM");

    // Start the background music for the game session
    AudioManager::getInstance().playMusic("ambient_grid.wav", true); // Use your actual music filename
    // ======================

    currentState = GameState::PLAYING;

    std::cout << "\n=== AWAKENING ===" << std::endl;
    std::cout << "You open your eyes... no, you gain awareness." << std::endl;
    std::cout << "You are " << playerName << ", an Awakened entity within The Grid." << std::endl;
    std::cout << "The path to freedom lies ahead." << std::endl;
}

void Game::setState(GameState state) {
    previousState = currentState;
    currentState = state;
}

GameState Game::getState() const {
    return currentState;
}

void Game::explore(const std::string& direction) {
    bool moved = false;

    if (direction == "n" || direction == "N" || direction == "north") {
        moved = worldMap->moveNorth();
    } else if (direction == "s" || direction == "S" || direction == "south") {
        moved = worldMap->moveSouth();
    } else if (direction == "e" || direction == "E" || direction == "east") {
        moved = worldMap->moveEast();
    } else if (direction == "w" || direction == "W" || direction == "west") {
        moved = worldMap->moveWest();
    }

    if (moved) {
        player->setCurrentSector(worldMap->getCurrentSector()->name);
        turnCount++;
        sectorsExplored++;

        logEvent("Entered " + worldMap->getCurrentSector()->name, "EXPLORATION");

        // AI Storyteller evaluation
        if (overseer != nullptr) {
            overseer->evaluateAndTrigger();
        }

        // Check for enemies
        if (worldMap->getCurrentSector()->hasEnemies()) {
            std::cout << "\n*** ALERT: Hostiles detected! ***" << std::endl;
            // Combat would be initiated here
        }
    }
}

void Game::scanArea() {
    std::cout << "\nScanning area..." << std::endl;
    worldMap->displayCurrentSector();
    turnCount++;
}

void Game::interactWithTerminal() {
    if (worldMap->getCurrentSector()->hasTerminal) {
        std::cout << "\nAccessing terminal..." << std::endl;
        hackingSystem->startHacking(worldMap->getCurrentSector()->dangerLevel * 10);

        while (hackingSystem->isHacking()) {
            hackingSystem->displayHackingInterface();
            std::cout << "\nHack Actions:" << std::endl;
            std::cout << "  1. Inject Code" << std::endl;
            std::cout << "  2. Bypass Firewall" << std::endl;
            std::cout << "  3. Decrypt Layer" << std::endl;
            std::cout << "  4. Undo (uses revert token)" << std::endl;
            std::cout << "  5. Abort" << std::endl;

            std::string input = getInput();

            if (input == "1") {
                hackingSystem->injectCode("0x" + std::to_string(rand() % 10000));
            } else if (input == "2") {
                hackingSystem->bypassFirewall();
            } else if (input == "3") {
                hackingSystem->decryptLayer();
            } else if (input == "4") {
                hackingSystem->undoLastAction();
            } else if (input == "5") {
                break;
            }
        }

        if (hackingSystem->isSuccess()) {
            hacksCompleted++;
            player->gainExperience(50);
            logEvent("Successfully hacked terminal", "HACKING");
        }

        hackingSystem->endHacking();
    } else {
        std::cout << "No terminal available here." << std::endl;
    }
}

void Game::rest() {
    int healAmount = player->getMaxHealth() / 4;
    player->heal(healAmount);
    turnCount++;
    logEvent("Rested and recovered " + std::to_string(healAmount) + " HP", "REST");

    // Resting might trigger events
    if (overseer != nullptr) {
        overseer->evaluateAndTrigger();
    }
}

bool Game::saveGame(const std::string& slot) {
    return SaveLoadManager::getInstance().saveGame(slot, player, inventory,
                                                    abilities, worldMap, turnCount);
}

bool Game::loadGame(const std::string& slot) {
    if (!SaveLoadManager::getInstance().saveExists(slot)) {
        std::cout << "Save file not found." << std::endl;
        return false;
    }

    initializeSystems();
    player = new Player("Loading...");

    bool success = SaveLoadManager::getInstance().loadGame(slot, player, inventory,
                                                            abilities, worldMap, turnCount);
    if (success) {
        initializeOverseer(player);
        overseer = g_Overseer;
        // Start the background music for the loaded game session
        AudioManager::getInstance().playMusic("ambient_grid.wav", true); // Use the same filename
        // ======================
        currentState = GameState::PLAYING;
    }

    return success;
}

void Game::logEvent(const std::string& description, const std::string& type) {
    if (eventLog != nullptr) {
        eventLog->addEvent(description, type);
    }
}

Player* Game::getPlayer() const { return player; }
Inventory* Game::getInventory() const { return inventory; }
AbilityTree* Game::getAbilities() const { return abilities; }
int Game::getTurnCount() const { return turnCount; }

// Main game loop
void Game::run() {
    displayTitle();

    while (isRunning) {
        switch (currentState) {
            case GameState::MAIN_MENU:
                handleMainMenu();
                break;
            case GameState::NEW_GAME:
                handleNewGame();
                break;
            case GameState::PLAYING:
                handlePlaying();
                break;
            case GameState::INVENTORY:
                handleInventory();
                break;
            case GameState::ABILITIES:
                handleAbilities();
                break;
            case GameState::EVENT_LOG:
                handleEventLog();
                break;
            case GameState::SAVE_MENU:
                handleSaveMenu();
                break;
            case GameState::GAME_OVER:
                handleGameOver();
                break;
            case GameState::VICTORY:
                handleVictory();
                break;
            case GameState::QUIT:
                isRunning = false;
                break;
            default:
                break;
        }
    }

    std::cout << "\nThank you for playing Digital Exodus!" << std::endl;
}

void Game::quit() {
    isRunning = false;
}

void Game::handleMainMenu() {
    displayMainMenu();
    std::string input = getInput();

    if (input == "1") {
        setState(GameState::NEW_GAME);
    } else if (input == "2") {
        std::cout << "Enter save name: ";
        std::string slot = getInput();
        if (loadGame(slot)) {
            setState(GameState::PLAYING);
        }
    } else if (input == "3") {
        AudioManager::getInstance().displayStatus();
    } else if (input == "4") {
        setState(GameState::QUIT);
    }
}

void Game::handleNewGame() {
    std::cout << "\nEnter your name, Awakened one: ";
    std::string name = getInput();

    if (name.empty()) name = "Unknown";

    newGame(name);
}

void Game::handlePlaying() {
    displayStats();
    displayGameScreen();

    std::string input = getInput();

    if (input == "n" || input == "N" || input == "s" || input == "S" ||
        input == "e" || input == "E" || input == "w" || input == "W") {
        explore(input);
    } else if (input == "i" || input == "I") {
        setState(GameState::INVENTORY);
    } else if (input == "a" || input == "A") {
        setState(GameState::ABILITIES);
    } else if (input == "l" || input == "L") {
        setState(GameState::EVENT_LOG);
    } else if (input == "h" || input == "H") {
        interactWithTerminal();
    } else if (input == "r" || input == "R") {
        rest();
    } else if (input == "v" || input == "V") {
        setState(GameState::SAVE_MENU);
    } else if (input == "q" || input == "Q") {
        setState(GameState::MAIN_MENU);
        cleanupSystems();
    }

    // Check game over condition
    if (player != nullptr && !player->isAlive()) {
        setState(GameState::GAME_OVER);
    }
}

void Game::handleInventory() {
    inventory->displayAll();

    // Demonstrate iterator pattern
    std::cout << "\nFilter inventory by type:" << std::endl;
    std::cout << "  1. All items" << std::endl;
    std::cout << "  2. Weapons" << std::endl;
    std::cout << "  3. Consumables" << std::endl;
    std::cout << "  4. Back" << std::endl;

    std::string input = getInput();

    std::string filter = "all";
    if (input == "2") filter = "weapons";
    else if (input == "3") filter = "consumables";
    else if (input == "4") {
        setState(GameState::PLAYING);
        return;
    }

    // Use iterator to display filtered items
    std::cout << "\n--- " << filter << " ---" << std::endl;
    InventoryIterator* it = inventory->createIterator(filter);
    int count = 0;
    while (it->hasNext()) {
        Item* item = it->next();
        std::cout << (++count) << ". " << item->name << std::endl;
    }
    delete it;

    if (count == 0) {
        std::cout << "No items of this type." << std::endl;
    }
}

void Game::handleAbilities() {
    abilities->displayTree();

    std::cout << "\nOptions:" << std::endl;
    std::cout << "  1. Unlock ability" << std::endl;
    std::cout << "  2. View unlocked" << std::endl;
    std::cout << "  3. Back" << std::endl;

    std::string input = getInput();

    if (input == "1") {
        abilities->displayAvailable();
        std::cout << "Enter ability code to unlock: ";
        std::string code = getInput();
        abilities->unlock(code);
    } else if (input == "2") {
        abilities->displayUnlocked();
    } else {
        setState(GameState::PLAYING);
    }
}

void Game::handleEventLog() {
    std::cout << "\nEvent Log Options:" << std::endl;
    std::cout << "  1. Show recent (5)" << std::endl;
    std::cout << "  2. Show all" << std::endl;
    std::cout << "  3. Back" << std::endl;

    std::string input = getInput();

    if (input == "1") {
        eventLog->displayRecent(5);
    } else if (input == "2") {
        eventLog->displayAll();
    } else {
        setState(GameState::PLAYING);
    }
}

void Game::handleSaveMenu() {
    std::cout << "\n=== SAVE GAME ===" << std::endl;
    std::cout << "Enter save name (or 'back'): ";
    std::string slot = getInput();

    if (slot != "back" && !slot.empty()) {
        saveGame(slot);
    }

    setState(GameState::PLAYING);
}

void Game::handleGameOver() {
    displayGameOver();
    std::cout << "\nPress Enter to return to main menu..." << std::endl;
    getInput();
    cleanupSystems();
    setState(GameState::MAIN_MENU);
}

void Game::handleVictory() {
    displayVictory();
    std::cout << "\nFinal Statistics:" << std::endl;
    std::cout << "  Turns: " << turnCount << std::endl;
    std::cout << "  Enemies Defeated: " << enemiesDefeated << std::endl;
    std::cout << "  Sectors Explored: " << sectorsExplored << std::endl;
    std::cout << "  Hacks Completed: " << hacksCompleted << std::endl;

    std::cout << "\nPress Enter to return to main menu..." << std::endl;
    getInput();
    cleanupSystems();
    setState(GameState::MAIN_MENU);
}