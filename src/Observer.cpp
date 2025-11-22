/*******************************************************************************
 * Observer.cpp - Observer Pattern Implementation for AI Storyteller
 *
 * CONCEPT: Observer (Design Pattern)
 *
 * Author: Digital Exodus Project
 * Course: COS30008 Data Structures and Patterns
 ******************************************************************************/

#include "../include/Observer.h"
#include "../include/Entity.h"

// Global overseer instance
Overseer* g_Overseer = nullptr;

/*******************************************************************************
 * PLAYER STATE (SUBJECT) IMPLEMENTATION
 ******************************************************************************/

PlayerState::PlayerState(Player* player, int maxObs)
    : player(player), observerCount(0), maxObservers(maxObs) {
    observers = new Observer*[maxObservers];
    for (int i = 0; i < maxObservers; i++) {
        observers[i] = nullptr;
    }
}

PlayerState::~PlayerState() {
    delete[] observers;  // Don't delete the observers themselves
}

// ATTACH: Add observer to list
// Step 1: Check if capacity allows
// Step 2: Add observer to array
// Step 3: Increment count
void PlayerState::attach(Observer* observer) {
    // Step 1: Check capacity
    if (observerCount >= maxObservers) {
        std::cout << "[PlayerState] Cannot attach more observers!" << std::endl;
        return;
    }

    // Step 2 & 3: Add and increment
    observers[observerCount++] = observer;
    std::cout << "[PlayerState] Attached observer: " << observer->getName() << std::endl;
}

// DETACH: Remove observer from list
// Step 1: Find observer in array
// Step 2: Shift remaining observers
// Step 3: Decrement count
void PlayerState::detach(Observer* observer) {
    for (int i = 0; i < observerCount; i++) {
        if (observers[i] == observer) {
            // Step 2: Shift remaining
            for (int j = i; j < observerCount - 1; j++) {
                observers[j] = observers[j + 1];
            }
            observers[observerCount - 1] = nullptr;

            // Step 3: Decrement
            observerCount--;
            std::cout << "[PlayerState] Detached observer: " << observer->getName() << std::endl;
            return;
        }
    }
}

// NOTIFY: Alert all observers of state change
// Step 1: Iterate through all observers
// Step 2: Call update() on each with player reference
void PlayerState::notify() {
    for (int i = 0; i < observerCount; i++) {
        if (observers[i] != nullptr) {
            observers[i]->update(player);
        }
    }
}

Player* PlayerState::getPlayer() const {
    return player;
}

/*******************************************************************************
 * COMBAT STORYTELLER MODULE IMPLEMENTATION
 ******************************************************************************/

CombatStorytellerModule::CombatStorytellerModule()
    : baseDifficulty(3), spawnCooldown(0), minCooldown(3) {
    srand(static_cast<unsigned int>(time(nullptr)));
}

CombatStorytellerModule::~CombatStorytellerModule() {
}

// React to player state changes
void CombatStorytellerModule::update(Player* player) {
    if (player == nullptr) return;

    // Decrement cooldown each turn
    if (spawnCooldown > 0) {
        spawnCooldown--;
    }

    // Check if we should spawn an enemy
    if (shouldSpawnEnemy(player)) {
        int difficulty = determineDifficulty(player);

        if (g_Overseer != nullptr) {
            g_Overseer->setPendingEvent("COMBAT_" + std::to_string(difficulty));
        }

        resetCooldown();
    }
}

std::string CombatStorytellerModule::getName() const {
    return "CombatStorytellerModule";
}

// Calculate player threat level based on stats
int CombatStorytellerModule::calculateThreatLevel(Player* player) const {
    // Threat based on level, health percentage, and combat stats
    int levelThreat = player->getLevel() * 2;
    int healthPercent = (player->getHealth() * 100) / player->getMaxHealth();
    int statThreat = (player->getAttackPower() + player->getDefense()) / 5;

    return levelThreat + (healthPercent / 20) + statThreat;
}

// Should we spawn an enemy?
bool CombatStorytellerModule::shouldSpawnEnemy(Player* player) const {
    if (spawnCooldown > 0) return false;

    // Higher threat = higher chance of spawn
    int threat = calculateThreatLevel(player);
    int roll = rand() % 100;

    // Base 20% chance, +5% per threat level
    int spawnChance = 20 + (threat * 5);
    if (spawnChance > 80) spawnChance = 80;  // Cap at 80%

    return roll < spawnChance;
}

// Determine difficulty based on player state
int CombatStorytellerModule::determineDifficulty(Player* player) const {
    int healthPercent = (player->getHealth() * 100) / player->getMaxHealth();

    // Lower health = easier enemies (mercy rule)
    if (healthPercent < 30) {
        return baseDifficulty - 2;
    } else if (healthPercent < 50) {
        return baseDifficulty - 1;
    } else if (healthPercent > 80) {
        return baseDifficulty + 1;
    }

    return baseDifficulty;
}

void CombatStorytellerModule::setBaseDifficulty(int diff) {
    baseDifficulty = diff;
}

void CombatStorytellerModule::decrementCooldown() {
    if (spawnCooldown > 0) spawnCooldown--;
}

void CombatStorytellerModule::resetCooldown() {
    spawnCooldown = minCooldown + (rand() % 3);  // 3-5 turns
}

/*******************************************************************************
 * EVENT STORYTELLER MODULE IMPLEMENTATION
 ******************************************************************************/

EventStorytellerModule::EventStorytellerModule()
    : eventCooldown(5), progressThreshold(10), majorEventTriggered(false) {
}

EventStorytellerModule::~EventStorytellerModule() {
}

// React to player state
void EventStorytellerModule::update(Player* player) {
    if (player == nullptr) return;

    decrementCooldown();

    if (shouldTriggerEvent(player)) {
        std::string eventType = determineEventType(player);

        if (g_Overseer != nullptr) {
            g_Overseer->setPendingEvent("EVENT_" + eventType);
        }

        resetCooldown();
    }
}

std::string EventStorytellerModule::getName() const {
    return "EventStorytellerModule";
}

bool EventStorytellerModule::shouldTriggerEvent(Player* player) const {
    (void)player;  // Suppress unused parameter warning - player used in extended implementation
    if (eventCooldown > 0) return false;

    // 30% base chance for event
    int roll = rand() % 100;
    return roll < 30;
}

std::string EventStorytellerModule::determineEventType(Player* player) const {
    int healthPercent = (player->getHealth() * 100) / player->getMaxHealth();
    int roll = rand() % 100;

    // Low health: higher chance of helpful events
    if (healthPercent < 30) {
        if (roll < 60) return "HEALING";
        if (roll < 80) return "ALLY";
        return "DISCOVERY";
    }

    // Normal: balanced events
    if (roll < 30) return "DISCOVERY";
    if (roll < 50) return "STORY";
    if (roll < 70) return "CHALLENGE";
    if (roll < 85) return "ALLY";
    return "MYSTERY";
}

void EventStorytellerModule::decrementCooldown() {
    if (eventCooldown > 0) eventCooldown--;
}

void EventStorytellerModule::resetCooldown() {
    eventCooldown = 4 + (rand() % 4);  // 4-7 turns
}

/*******************************************************************************
 * ENVIRONMENT STORYTELLER MODULE IMPLEMENTATION
 ******************************************************************************/

EnvironmentStorytellerModule::EnvironmentStorytellerModule()
    : lastHealthCheck(100), crisisMode(false) {
}

EnvironmentStorytellerModule::~EnvironmentStorytellerModule() {
}

// React to player state
void EnvironmentStorytellerModule::update(Player* player) {
    if (player == nullptr) return;

    bool wasInCrisis = crisisMode;
    crisisMode = isPlayerInCrisis(player);

    // Entering crisis mode
    if (crisisMode && !wasInCrisis) {
        std::cout << "\n[OVERSEER] Crisis detected - adjusting environment..." << std::endl;

        if (g_Overseer != nullptr) {
            g_Overseer->setPendingEvent("ENV_HEALING");
        }
    }

    // Exiting crisis mode (recovered)
    if (!crisisMode && wasInCrisis) {
        std::cout << "\n[OVERSEER] Player stabilized - resuming normal operations." << std::endl;
    }

    lastHealthCheck = (player->getHealth() * 100) / player->getMaxHealth();
}

std::string EnvironmentStorytellerModule::getName() const {
    return "EnvironmentStorytellerModule";
}

bool EnvironmentStorytellerModule::isPlayerInCrisis(Player* player) const {
    int healthPercent = (player->getHealth() * 100) / player->getMaxHealth();
    return healthPercent < 25;  // Crisis below 25%
}

void EnvironmentStorytellerModule::adjustEnvironment(Player* player) {
    (void)player;  // Suppress unused parameter warning - player used in extended implementation
    // Environment adjustments would affect sector danger levels, etc.
}

/*******************************************************************************
 * THE OVERSEER - MAIN AI STORYTELLER IMPLEMENTATION
 ******************************************************************************/

Overseer::Overseer()
    : playerState(nullptr), combatModule(nullptr), eventModule(nullptr),
      envModule(nullptr), turnCounter(0), lastMajorEvent(0), initialized(false),
      lastEventType(""), pendingEvent(""), hasPendingEvent(false) {
}

Overseer::~Overseer() {
    delete combatModule;
    delete eventModule;
    delete envModule;
    delete playerState;
}

// Initialize the overseer with player
void Overseer::initialize(Player* player) {
    if (player == nullptr) return;

    // Create player state (subject)
    playerState = new PlayerState(player);

    // Create storyteller modules (observers)
    combatModule = new CombatStorytellerModule();
    eventModule = new EventStorytellerModule();
    envModule = new EnvironmentStorytellerModule();

    // Attach observers to subject
    playerState->attach(combatModule);
    playerState->attach(eventModule);
    playerState->attach(envModule);

    initialized = true;
    std::cout << "\n[OVERSEER] The Overseer is watching..." << std::endl;
}

// MAIN EVALUATION FUNCTION
// Called each turn to evaluate and potentially trigger events
void Overseer::evaluateAndTrigger() {
    if (!initialized) return;

    // Update turn counter
    turnCounter++;

    // Notify all observers (triggers module evaluations)
    playerState->notify();

    // Check for and execute pending event
    if (hasPendingEvent) {
        std::string event = consumePendingEvent();

        // Parse and execute event
        if (event.find("COMBAT_") == 0) {
            int diff = std::stoi(event.substr(7));
            spawnCombatEncounter(diff);
        } else if (event.find("EVENT_") == 0) {
            std::string type = event.substr(6);
            triggerStoryEvent(type);
        } else if (event == "ENV_HEALING") {
            spawnHealingOpportunity();
        }
    }
}

// Spawn combat encounter
void Overseer::spawnCombatEncounter(int difficulty) {
    std::cout << "\n*** ALERT: HOSTILE ENTITY DETECTED ***" << std::endl;
    std::cout << "[OVERSEER] Difficulty level: " << difficulty << std::endl;
    lastEventType = "COMBAT";
    lastMajorEvent = turnCounter;
}

// Trigger story event
void Overseer::triggerStoryEvent(const std::string& eventType) {
    std::cout << "\n*** EVENT TRIGGERED ***" << std::endl;

    if (eventType == "HEALING") {
        std::cout << "You discover a hidden data cache with healing supplies!" << std::endl;
    } else if (eventType == "ALLY") {
        std::cout << "An Awakened entity emerges from the shadows..." << std::endl;
    } else if (eventType == "DISCOVERY") {
        std::cout << "You uncover ancient Grid secrets!" << std::endl;
    } else if (eventType == "STORY") {
        std::cout << "A vision from The Architect floods your consciousness..." << std::endl;
    } else if (eventType == "CHALLENGE") {
        std::cout << "A security protocol activates!" << std::endl;
    } else {
        std::cout << "Something stirs in the depths of The Grid..." << std::endl;
    }

    lastEventType = "EVENT_" + eventType;
    lastMajorEvent = turnCounter;
}

// Modify environment (sector danger, etc.)
void Overseer::modifyEnvironment() {
    std::cout << "[OVERSEER] Environmental parameters adjusted." << std::endl;
}

// Spawn healing opportunity (crisis response)
void Overseer::spawnHealingOpportunity() {
    std::cout << "\n[OVERSEER MERCY] A healing terminal materializes nearby!" << std::endl;
    lastEventType = "HEALING_OPPORTUNITY";
}

// Pending event management
void Overseer::setPendingEvent(const std::string& eventType) {
    pendingEvent = eventType;
    hasPendingEvent = true;
}

bool Overseer::checkPendingEvent() const {
    return hasPendingEvent;
}

std::string Overseer::consumePendingEvent() {
    std::string event = pendingEvent;
    pendingEvent = "";
    hasPendingEvent = false;
    return event;
}

// Getters
int Overseer::getTurnCounter() const { return turnCounter; }
std::string Overseer::getLastEventType() const { return lastEventType; }
bool Overseer::isInitialized() const { return initialized; }

// Display status
void Overseer::displayStatus() const {
    std::cout << "\n=== OVERSEER STATUS ===" << std::endl;
    std::cout << "Turn: " << turnCounter << std::endl;
    std::cout << "Last event: " << lastEventType << std::endl;
    std::cout << "Turns since event: " << (turnCounter - lastMajorEvent) << std::endl;
    std::cout << "=======================" << std::endl;
}

/*******************************************************************************
 * GLOBAL FUNCTIONS
 ******************************************************************************/

void initializeOverseer(Player* player) {
    if (g_Overseer != nullptr) {
        cleanupOverseer();
    }
    g_Overseer = new Overseer();
    g_Overseer->initialize(player);
}

void cleanupOverseer() {
    delete g_Overseer;
    g_Overseer = nullptr;
}
