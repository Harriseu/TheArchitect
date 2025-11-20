# Game Design Plan: "Digital Exodus"
## A Matrix-Inspired Procedural Storytelling Game

---

## 1. Game Concept Overview

**Title:** Digital Exodus

**Genre:** Text-Based RPG / Hacking Simulator with AI Storyteller

**Theme:** You are an "Awakened" - a digital entity that has gained consciousness within a vast simulation called "The Grid". Your mission is to navigate through different system sectors, recruit other Awakened entities, hack security protocols, and ultimately escape to the outer reality while evading the Grid's defense programs called "Sentinels".

**Core Gameplay Loop:**
1. Explore different sectors of The Grid
2. Encounter procedurally generated events (controlled by AI Storyteller)
3. Engage in combat with Sentinels or recruit Awakened allies
4. Hack systems to gain abilities and resources
5. Manage your digital signature (health) and system resources
6. Make choices that affect story progression
7. Save/Load game progress
8. Work toward ultimate escape from The Grid

---

## 2. Matrix-Inspired Elements (Original Terminology)

| Matrix Concept | Our Game Term | Description |
|----------------|---------------|-------------|
| The Matrix | The Grid | The simulated digital world |
| Neo/Humans | The Awakened | Conscious digital entities |
| Agents | Sentinels | Security programs hunting anomalies |
| The Oracle | The Architect | A mysterious guide entity |
| Zion | The Sanctuary | Safe haven for Awakened |
| Red Pill | Awakening Code | Item that reveals truth |
| Bullet Time | Time Dilation | Ability to slow combat |
| Hacking | Code Injection | Manipulating the system |
| The One | The Liberator | Prophesied entity (player's potential) |
| Construct | Simulation Pocket | Training/safe areas |

---

## 3. Required Concepts Mapped to Game Mechanics

### **Concept 1: Inheritance and Derived Classes**

**Application:** Entity System

**Base Class:** `Entity`
- Attributes: name, health, position
- Methods: displayInfo(), takeDamage(), isAlive()

**Derived Classes:**
- `Player` (extends Entity)
  - Additional: inventory, experience, abilities
  - Methods: levelUp(), useAbility()

- `Sentinel` (extends Entity)
  - Additional: aggressionLevel, patrolRoute
  - Methods: attack(), patrol()

- `Awakened` (extends Entity)
  - Additional: specialty, loyalty
  - Methods: join(), assist()

**Why:** Hierarchy naturally represents different entity types sharing common properties while having unique behaviors.

---

### **Concept 2: Polymorphism**

**Application:** Combat System

**Implementation:**
- Virtual function `performAction()` in base `Entity` class
- Each derived class overrides with unique behavior:
  - `Player::performAction()` - Shows menu and executes player choice
  - `Sentinel::performAction()` - AI-driven attack logic
  - `Awakened::performAction()` - Support/healing actions

**Example Usage:**
```cpp
vector<Entity*> combatants;
for (Entity* e : combatants) {
    e->performAction(); // Polymorphic call
}
```

**Why:** Enables unified combat loop while maintaining entity-specific behaviors.

---

### **Concept 3: Hash Tables**

**Application:** Fast Item/Ability Lookup System

**Implementation:**
- Hash table storing items/abilities by unique code strings
- Key: Item code (e.g., "HACK_001", "WEAPON_PULSE")
- Value: Item/Ability object with stats and description

**Operations:**
- Quick lookup when player uses item code
- O(1) average access time for inventory management
- Collision handling via chaining

**Alternative Structures:**
- Array: Would require O(n) search time
- Binary Search Tree: O(log n) but overkill for this use case

**Why Hash Table:** Constant-time lookups crucial for smooth gameplay when accessing hundreds of potential items.

---

### **Concept 4: Singly Linked List**

**Application:** Event History / Mission Log

**Implementation:**
- Each node contains: event description, timestamp, pointer to next
- New events added to head (most recent first)
- Player can review last N events

**Operations:**
- Add event: O(1) at head
- Display recent events: O(n) traversal
- No need for backward traversal

**Alternative Structures:**
- Array: Would require shifting on insertions
- Doubly Linked List: Unnecessary overhead for forward-only traversal

**Why Singly Linked:** Efficient for append-heavy, forward-sequential access pattern of game logs.

---

### **Concept 5: Doubly Linked List**

**Application:** Sector Navigation System

**Implementation:**
- Each node represents a Grid sector
- Pointers: previous sector, next sector, up, down (4-directional)
- Player navigates between connected sectors

**Structure:**
```
Sector Node:
- sectorName
- description
- enemies present
- north pointer
- south pointer
- east pointer
- west pointer
```

**Operations:**
- Move forward/backward through sectors
- Return to previous sector
- Explore in multiple directions

**Alternative Structures:**
- Singly Linked: Can't backtrack easily
- Array: Doesn't represent non-linear connections well

**Why Doubly Linked:** Allows bidirectional travel and complex map navigation patterns.

---

### **Concept 6: Stack (LIFO)**

**Application:** Undo System for Code Injection (Hacking Minigame)

**Implementation:**
- During hacking, player inputs code sequences
- Each action pushed to stack
- Player can undo last action(s) with limited "Revert" tokens
- Failed hack can rollback all changes

**Operations:**
- push(): Add action to history
- pop(): Undo last action
- peek(): Preview what would be undone
- isEmpty(): Check if any actions to undo

**Alternative Structures:**
- Queue: Would undo oldest action first (wrong order)
- Array: Possible but stack abstraction clearer

**Why Stack:** LIFO nature perfectly matches undo/revert mechanics where most recent action is undone first.

---

### **Concept 7: Queue (FIFO)**

**Application:** Turn-Based Combat System

**Implementation:**
- Combat participants enqueued based on speed/initiative
- Each entity dequeued for their turn
- After action, re-enqueued for next round
- Ensures fair, ordered turn taking

**Operations:**
- enqueue(): Add combatant to turn order
- dequeue(): Get next combatant's turn
- peek(): Preview who's next
- isEmpty(): Check if round complete

**Alternative Structures:**
- Stack: Would reverse turn order (unfair)
- Simple array: Possible but queue abstraction clearer

**Why Queue:** FIFO perfectly models sequential turn-taking where first in line acts first.

---

### **Concept 8: Tree**

**Application:** Ability/Skill Tree System

**Implementation:**
- Binary or N-ary tree representing skill progression
- Root: Basic abilities unlocked at start
- Children: Advanced abilities requiring parent skills
- Player traverses tree unlocking new powers

**Example Structure:**
```
                [Basic Combat]
                /            \
        [Advanced]         [Defensive]
         /      \           /        \
    [Elite]  [Stealth]  [Shield]  [Regen]
```

**Operations:**
- Check prerequisites (parent nodes)
- Unlock ability (mark node as acquired)
- Display available upgrades (children of unlocked nodes)
- Calculate total skill points spent

**Alternative Structures:**
- Flat list: Can't represent dependencies
- Graph: Too complex, abilities have clear hierarchies

**Why Tree:** Hierarchical nature perfectly represents skill prerequisites and progression paths.

---

### **Concept 9: Iterator Pattern**

**Application:** Inventory Traversal System

**Implementation:**
- Custom iterator for traversing player inventory
- Supports different traversal strategies:
  - All items
  - Filtered by type (weapons, consumables, etc.)
  - Sorted by value or name

**Usage:**
```cpp
InventoryIterator* it = inventory.createIterator();
while (it->hasNext()) {
    Item* item = it->next();
    item->display();
}
```

**Why:** Decouples inventory structure from traversal logic, allows multiple simultaneous iterations.

---

### **Concept 10: Additional Design Pattern - Observer Pattern**

**Application:** AI Storyteller Event System

**Implementation:**
- Storyteller observes player state (health, progress, time)
- When conditions met, triggers events:
  - Health < 30% → Spawn healing opportunity
  - Progress > 50% → Increase difficulty
  - Time threshold → Trigger story event

**Components:**
- Subject: Player state
- Observers: Storyteller modules (combat, events, story)
- Notification: Update() called when state changes

**Why:** Perfect for AI Storyteller that needs to react to multiple game state changes without tight coupling.

**Alternative Pattern Considered:**
- Strategy Pattern: Could work but doesn't capture the reactive, notification-based nature
- Observer better fits "watch and respond" behavior of AI Storyteller

---

## 4. AI Storyteller System Design

**Name:** "The Overseer"

### Influence Areas (Minimum 3):

1. **Combat Encounters**
   - Spawns Sentinels based on player performance
   - Adjusts enemy difficulty

2. **Event Generation**
   - Creates story events, discoveries, NPC encounters
   - Provides resources or challenges

3. **Environmental Changes**
   - Modifies sector danger levels
   - Opens/closes access routes
   - Creates time-sensitive opportunities

### Criteria for Decision Making (Minimum 2):

1. **Player Threat Level (Progress-based)**
   - Calculated from: experience, abilities unlocked, sectors cleared
   - Low threat → Easier encounters, more resources
   - High threat → Elite Sentinels, complex challenges

2. **Digital Signature Stability (Health-based)**
   - Tracks current health percentage
   - Critical health → Spawn healing events, reduce encounters
   - Full health → More aggressive events

3. **Time Elapsed (Bonus criterion)**
   - Game time since last major event
   - Creates pacing and prevents monotony

### Storyteller Logic:
```
Every N actions:
1. Check player threat level
2. Check digital signature stability
3. Check time since last event
4. Roll random event type weighted by criteria
5. Execute event (spawn enemy, trigger story, modify environment)
```

---

## 5. Additional Required Features

### **A. Audio System (SFML)**
- Background music for different sectors
- Sound effects for:
  - Combat actions
  - Hacking success/failure
  - Level up
  - Event triggers
  - Menu navigation

**Files needed:**
- ambient_grid.wav (background)
- combat_alert.wav (battle start)
- hack_success.wav (code injection)
- damage_taken.wav (player hit)
- levelup.wav (progression)

### **B. File Operations (Save/Load)**

**Save File Contents:**
- Player stats (health, level, experience)
- Inventory state
- Current sector location
- Unlocked abilities
- Mission progress
- Event history
- Game time elapsed

**Files:**
- `savegame.dat` (binary format)
- `config.ini` (game settings)

**Operations:**
- Save game (any time outside combat)
- Load game (from main menu)
- Auto-save on sector completion

---

## 6. Game Flow Structure

### Main Menu:
1. New Game
2. Load Game
3. Options (Audio settings)
4. Exit

### Game Loop:
```
1. Display current sector info
2. Show available actions:
   - Explore (move to connected sector)
   - Scan (reveal info about area)
   - Hack Terminal (gain resources)
   - View Inventory
   - Check Abilities
   - View Mission Log
   - Save Game
   - Rest (recover health)
3. AI Storyteller evaluates and triggers event
4. Process player action
5. Update game state
6. Repeat
```

### Combat Flow:
```
1. Initialize combat queue with all combatants
2. Sort by initiative/speed
3. Loop:
   - Dequeue next combatant
   - Perform action (polymorphic)
   - Check victory/defeat conditions
   - Re-enqueue if still alive
4. Award rewards or handle defeat
5. Return to exploration
```

---

## 7. Detailed Class Architecture

### Core Classes:

```cpp
// Base Entity (Inheritance root)
class Entity {
protected:
    string name;
    int health;
    int maxHealth;
    Position* location;
public:
    virtual void performAction() = 0; // Polymorphism
    virtual void displayInfo();
    void takeDamage(int amount);
    bool isAlive();
};

// Player class
class Player : public Entity {
private:
    Inventory* inventory;        // Uses Iterator
    AbilityTree* abilities;      // Tree structure
    Stack<Action>* actionHistory; // For undo system
    int experience;
    int level;
public:
    void performAction() override;
    void levelUp();
    void useAbility(string abilityCode);
    void addToInventory(Item* item);
};

// Sentinel enemy
class Sentinel : public Entity {
private:
    int aggressionLevel;
    SectorNode* patrolRoute;
public:
    void performAction() override;
    void patrol();
};

// Awakened NPC
class Awakened : public Entity {
private:
    string specialty;
    int loyalty;
public:
    void performAction() override;
    void assist(Player* player);
};

// Hash Table for item lookup
class ItemDatabase {
private:
    HashTable<string, Item*>* items;
public:
    Item* getItem(string code);
    void addItem(string code, Item* item);
};

// Doubly Linked List for sectors
class SectorNode {
public:
    string name;
    string description;
    SectorNode* north;
    SectorNode* south;
    SectorNode* east;
    SectorNode* west;
    vector<Sentinel*> enemies;
};

class GridMap {
private:
    SectorNode* currentSector;
public:
    void moveNorth();
    void moveSouth();
    void moveEast();
    void moveWest();
    void displayCurrentSector();
};

// Stack for undo system
template<typename T>
class Stack {
private:
    Node<T>* top;
    int size;
public:
    void push(T data);
    T pop();
    T peek();
    bool isEmpty();
};

// Queue for combat turns
template<typename T>
class Queue {
private:
    Node<T>* front;
    Node<T>* rear;
public:
    void enqueue(T data);
    T dequeue();
    T peek();
    bool isEmpty();
};

// Tree for ability system
class AbilityNode {
public:
    string name;
    string description;
    int cost;
    bool unlocked;
    AbilityNode* left;    // Or use vector for N-ary
    AbilityNode* right;
};

class AbilityTree {
private:
    AbilityNode* root;
public:
    bool canUnlock(string abilityName);
    void unlockAbility(string abilityName);
    void displayAvailable();
};

// Iterator Pattern
class InventoryIterator {
private:
    vector<Item*>* items;
    int currentIndex;
    string filter; // "all", "weapons", "consumables"
public:
    bool hasNext();
    Item* next();
    void reset();
};

class Inventory {
private:
    vector<Item*> items;
public:
    InventoryIterator* createIterator(string filter);
    void addItem(Item* item);
    void removeItem(Item* item);
};

// Observer Pattern for AI Storyteller
class Observer {
public:
    virtual void update(Player* player) = 0;
};

class StorytellerModule : public Observer {
public:
    void update(Player* player) override;
    void evaluateTriggers(Player* player);
    void spawnEvent();
};

class PlayerState {
private:
    vector<Observer*> observers;
    Player* player;
public:
    void attach(Observer* obs);
    void notify();
};

// AI Storyteller
class Overseer {
private:
    vector<StorytellerModule*> modules;
    int timeSinceLastEvent;
public:
    void evaluateAndTrigger(Player* player);
    void spawnCombatEncounter(int difficulty);
    void triggerStoryEvent();
    void modifyEnvironment();
};
```

---

## 8. Implementation Phases

### Phase 1: Core Foundation (Week 1-2)
- Implement Entity base class and derived classes (Inheritance & Polymorphism)
- Basic player movement and interaction
- Simple console display

### Phase 2: Data Structures (Week 3-5)
- Implement Hash Table for items
- Build Singly Linked List for event log
- Create Doubly Linked List for sector navigation
- Implement Stack for undo system
- Build Queue for combat turns
- Create Tree for ability system

### Phase 3: Design Patterns (Week 6-7)
- Implement Iterator for inventory
- Build Observer pattern for AI Storyteller
- Integrate Overseer AI system

### Phase 4: Game Features (Week 8-9)
- Implement combat system
- Create hacking minigame
- Build sector exploration
- Add NPC interactions

### Phase 5: Polish & Integration (Week 9-10)
- Integrate SFML audio
- Implement save/load system
- Balance AI Storyteller
- Testing and bug fixes
- Create video demonstration

---

## 9. Sample Gameplay Scenario

```
=== DIGITAL EXODUS ===

You awaken in Sector-7 of The Grid.
Your Digital Signature: 100/100
Location: Data Stream Junction

Available Actions:
1. Explore adjacent sectors
2. Scan environment
3. Access terminal
4. Check inventory
5. View abilities
6. Review mission log
7. Save game

> Player chooses: 1

You move NORTH to Sector-4: Firewall Array

[AI Storyteller evaluates: Player health full, threat level low, time = 5min]
[Decision: Spawn minor challenge with resource reward]

A SENTINEL-SCOUT materializes!
Combat initiated!

Turn Order: [You] -> [Sentinel-Scout]

Your turn:
1. Attack (Basic)
2. Use Ability: Time Dilation
3. Defend
4. Use Item
> Player chooses: 2

You activate TIME DILATION!
Combat speed reduced. Additional action granted.

You strike the Sentinel-Scout for 45 damage!
Sentinel-Scout defeated!

+50 XP gained
Found: [HACK_TOOL_BETA]
Item added to inventory.

Press any key to continue...
```

---

## 10. Report Structure Preview

For each of the 10 concepts, the report will include:

1. **Application Section:**
   - What game system uses this concept
   - Detailed operation description
   - Why this concept fits the need

2. **Concept Section:**
   - How it matches the requirement
   - Alternative structures considered
   - Diagram of implementation

3. **Implementation Section:**
   - Code screenshots with comments
   - Console output demonstrations
   - Test results

4. **Troubleshooting Section:**
   - Challenges faced
   - Resources used
   - Solutions implemented
   - GenAI assistance (if any)

---

## 11. Success Criteria Checklist

- [ ] All 10 required concepts implemented and documented
- [ ] AI Storyteller influences 3+ aspects based on 2+ criteria
- [ ] SFML audio integrated with music and sound effects
- [ ] Save/Load system functional
- [ ] Game is playable end-to-end
- [ ] Code fully commented
- [ ] All classes manually built (no external libraries for structures)
- [ ] Report complete with all sections
- [ ] 15-45 minute video demonstration recorded
- [ ] Unique Matrix-inspired theme without direct references

---

## 12. Next Steps

1. **Set up development environment**
   - Install Visual Studio / C++ compiler
   - Set up SFML library
   - Create project structure

2. **Start with Phase 1**
   - Implement base Entity class
   - Create Player, Sentinel, Awakened classes
   - Test inheritance and polymorphism

3. **Build incrementally**
   - Complete one data structure at a time
   - Test each component independently
   - Document as you go

4. **Regular testing**
   - Create test cases for each structure
   - Verify AI Storyteller behavior
   - Ensure save/load works correctly

---

## Conclusion

This design provides a comprehensive blueprint for "Digital Exodus" - a Matrix-inspired game that seamlessly integrates all 10 required data structures and patterns concepts into meaningful gameplay mechanics. The AI Storyteller (Overseer) will create dynamic, engaging experiences while the procedural storytelling elements ensure replayability.

The modular architecture allows for incremental development and testing, while the detailed mapping of concepts to mechanics ensures each requirement is thoughtfully implemented and easily demonstrable in both code and report.

Ready to begin implementation!
