/*******************************************************************************
 * Observer.h - Observer Pattern for AI Storyteller System
 *
 * CONCEPT: Observer (Design Pattern)
 *
 * This pattern implements the AI Storyteller (Overseer) that watches
 * player state and triggers events based on conditions.
 * When player state changes, all observers are notified.
 *
 * WHY OBSERVER:
 * - AI Storyteller needs to react to multiple game state changes
 * - Loose coupling between player state and event triggers
 * - Multiple modules can observe independently
 * - Perfect for "watch and respond" behavior
 *
 * Alternative Considered: Strategy Pattern
 * - Strategy better for swappable algorithms
 * - Observer better for reactive, notification-based systems
 *
 * Author: Alif Harriz Jeffery | 102782711
 * Course: COS30008 Data Structures and Patterns
 ******************************************************************************/

#ifndef OBSERVER_H
#define OBSERVER_H

#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>

// Forward declarations
class Player;
struct SectorNode;

/*******************************************************************************
 * CONCEPT: OBSERVER PATTERN - Observer Interface
 *
 * All observers implement this interface to receive notifications.
 ******************************************************************************/
class Observer {
public:
    virtual ~Observer() {}

    // update: Called when subject state changes
    // Each observer reacts differently to player state
    virtual void update(Player* player) = 0;

    // getName: Identify the observer
    virtual std::string getName() const = 0;
};

/*******************************************************************************
 * CONCEPT: OBSERVER PATTERN - Subject (Observable)
 *
 * PlayerState is the Subject that observers watch.
 * When state changes, all attached observers are notified.
 ******************************************************************************/
class PlayerState {
private:
    Observer** observers;       // Array of observer pointers
    int observerCount;          // Number of attached observers
    int maxObservers;           // Maximum capacity
    Player* player;             // Reference to player

public:
    PlayerState(Player* player, int maxObs = 10);
    ~PlayerState();

    // Attach: Add observer to list
    // Step 1: Check if capacity allows
    // Step 2: Add observer to array
    // Step 3: Increment count
    void attach(Observer* observer);

    // Detach: Remove observer from list
    // Step 1: Find observer in array
    // Step 2: Shift remaining observers
    // Step 3: Decrement count
    void detach(Observer* observer);

    // Notify: Alert all observers of state change
    // Step 1: Iterate through all observers
    // Step 2: Call update() on each with player reference
    void notify();

    // Get player reference
    Player* getPlayer() const;
};

/*******************************************************************************
 * STORYTELLER MODULES - Concrete Observers
 *
 * Each module watches different aspects of player state
 * and triggers appropriate events.
 ******************************************************************************/

/*******************************************************************************
 * Combat Storyteller Module
 *
 * Influences: Enemy spawning, combat difficulty
 * Criteria: Player threat level, recent combat history
 ******************************************************************************/
class CombatStorytellerModule : public Observer {
private:
    int baseDifficulty;         // Base difficulty level
    int spawnCooldown;          // Turns since last spawn
    int minCooldown;            // Minimum turns between spawns

public:
    CombatStorytellerModule();
    ~CombatStorytellerModule() override;

    // React to player state changes
    // Step 1: Calculate player threat level
    // Step 2: Determine if spawn is needed
    // Step 3: Adjust difficulty based on player health/level
    void update(Player* player) override;

    std::string getName() const override;

    // Combat-specific methods
    int calculateThreatLevel(Player* player) const;
    bool shouldSpawnEnemy(Player* player) const;
    int determineDifficulty(Player* player) const;

    // Setters
    void setBaseDifficulty(int diff);
    void decrementCooldown();
    void resetCooldown();
};

/*******************************************************************************
 * Event Storyteller Module
 *
 * Influences: Story events, discoveries, NPC encounters
 * Criteria: Progress level, time elapsed
 ******************************************************************************/
class EventStorytellerModule : public Observer {
private:
    int eventCooldown;          // Turns since last event
    int progressThreshold;      // Progress needed for major events
    bool majorEventTriggered;   // Whether big event occurred recently

public:
    EventStorytellerModule();
    ~EventStorytellerModule() override;

    // React to player state
    // Step 1: Check time since last event
    // Step 2: Evaluate progress milestones
    // Step 3: Trigger appropriate event type
    void update(Player* player) override;

    std::string getName() const override;

    // Event-specific methods
    bool shouldTriggerEvent(Player* player) const;
    std::string determineEventType(Player* player) const;
    void decrementCooldown();
    void resetCooldown();
};

/*******************************************************************************
 * Environment Storyteller Module
 *
 * Influences: Sector danger levels, access routes, opportunities
 * Criteria: Player health, sector history
 ******************************************************************************/
class EnvironmentStorytellerModule : public Observer {
private:
    int lastHealthCheck;        // Last recorded health percentage
    bool crisisMode;            // Whether player is in danger

public:
    EnvironmentStorytellerModule();
    ~EnvironmentStorytellerModule() override;

    // React to player state
    // Step 1: Check health percentage
    // Step 2: If critical, enable crisis mode (spawn healing)
    // Step 3: If healthy, increase challenge
    void update(Player* player) override;

    std::string getName() const override;

    // Environment-specific methods
    bool isPlayerInCrisis(Player* player) const;
    void adjustEnvironment(Player* player);
};

/*******************************************************************************
 * THE OVERSEER - Main AI Storyteller
 *
 * Coordinates all storyteller modules and makes high-level decisions.
 * This is the "brain" of the procedural storytelling system.
 ******************************************************************************/
class Overseer {
private:
    PlayerState* playerState;               // Subject being observed
    CombatStorytellerModule* combatModule;
    EventStorytellerModule* eventModule;
    EnvironmentStorytellerModule* envModule;

    int turnCounter;            // Total turns elapsed
    int lastMajorEvent;         // Turn of last major event
    bool initialized;           // Whether overseer is ready

    // Event tracking
    std::string lastEventType;
    std::string pendingEvent;
    bool hasPendingEvent;

public:
    Overseer();
    ~Overseer();

    // Initialize with player
    void initialize(Player* player);

    // Main evaluation function
    // Called each turn to evaluate and potentially trigger events
    // Step 1: Update turn counter
    // Step 2: Notify all observers (triggers module evaluations)
    // Step 3: Check for pending events from modules
    // Step 4: Execute highest priority event
    void evaluateAndTrigger();

    // Event execution
    void spawnCombatEncounter(int difficulty);
    void triggerStoryEvent(const std::string& eventType);
    void modifyEnvironment();
    void spawnHealingOpportunity();

    // Pending event management
    void setPendingEvent(const std::string& eventType);
    bool checkPendingEvent() const;
    std::string consumePendingEvent();

    // Getters
    int getTurnCounter() const;
    std::string getLastEventType() const;
    bool isInitialized() const;

    // Debug/Display
    void displayStatus() const;
};

/*******************************************************************************
 * Global Overseer instance (Singleton-like access)
 ******************************************************************************/
extern Overseer* g_Overseer;

// Initialize global overseer
void initializeOverseer(Player* player);

// Cleanup global overseer
void cleanupOverseer();

#endif // OBSERVER_H