/*******************************************************************************
 * LinkedList.h - Singly and Doubly Linked List Implementations
 *
 * CONCEPT: Singly Linked List, Doubly Linked List
 *
 * SINGLY LINKED LIST - Event History/Mission Log
 * - New events added at head (most recent first)
 * - Forward-only traversal for displaying recent events
 * - O(1) insertion at head, O(n) traversal
 *
 * WHY SINGLY LINKED:
 * - No need for backward traversal in event log
 * - Simpler implementation, less memory overhead
 * - Perfect for append-heavy, forward-sequential access
 *
 * DOUBLY LINKED LIST - Sector Navigation
 * - Each node represents a Grid sector
 * - 4-directional navigation (N, S, E, W)
 * - Allows backtracking through visited sectors
 *
 * WHY DOUBLY LINKED:
 * - Need bidirectional travel between sectors
 * - Player must be able to return to previous areas
 * - Complex interconnections between sectors
 *
 * Author: Alif Harriz Jeffery | 102782711
 * Course: COS30008 Data Structures and Patterns
 ******************************************************************************/

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <string>
#include <ctime>
#include <iostream>

/*******************************************************************************
 * CONCEPT: SINGLY LINKED LIST - Event Log System
 ******************************************************************************/

// Event node for singly linked list
struct EventNode {
    std::string description;    // What happened
    std::string timestamp;      // When it happened
    std::string eventType;      // Category (COMBAT, STORY, SYSTEM, etc.)
    EventNode* next;            // Pointer to next event (older event)

    EventNode(const std::string& desc, const std::string& type);
};

/*******************************************************************************
 * SINGLY LINKED LIST IMPLEMENTATION - EventLog
 *
 * Operations:
 * - addEvent(desc, type): Insert at head - O(1)
 * - displayRecent(n): Show last n events - O(n)
 * - clear(): Remove all events - O(n)
 * - getEventCount(): Return total events - O(1)
 *
 * Structure:
 * HEAD -> [Event3] -> [Event2] -> [Event1] -> nullptr
 * (newest)                            (oldest)
 ******************************************************************************/
class EventLog {
private:
    EventNode* head;            // Points to most recent event
    int eventCount;             // Total number of events

public:
    // Constructor - initializes empty log
    EventLog();

    // Destructor - frees all nodes
    ~EventLog();

    // Add Event: Insert new event at head
    // Step 1: Create new EventNode with description and type
    // Step 2: Set new node's next pointer to current head
    // Step 3: Update head to point to new node
    // Step 4: Increment event count
    void addEvent(const std::string& description, const std::string& type);

    // Display Recent: Show last n events
    // Step 1: Start at head (most recent)
    // Step 2: Traverse and display up to n events
    // Step 3: Stop at nullptr or after n events
    void displayRecent(int count) const;

    // Display All: Show entire event history
    void displayAll() const;

    // Clear: Remove all events
    // Step 1: Traverse list from head
    // Step 2: Delete each node
    // Step 3: Set head to nullptr, count to 0
    void clear();

    // Getters
    int getEventCount() const;
    bool isEmpty() const;

    // Get events as string array (for save system)
    std::string* getEventsAsArray(int& outCount) const;
};

/*******************************************************************************
 * CONCEPT: DOUBLY LINKED LIST - Sector Navigation System
 ******************************************************************************/

// Forward declaration
class Sentinel;

// Sector node for doubly linked list (4-directional)
struct SectorNode {
    std::string name;           // Sector identifier
    std::string description;    // Area description
    int dangerLevel;            // 1-10 danger rating
    bool visited;               // Has player been here
    bool hasTerminal;           // Hackable terminal present

    // 4-directional pointers (doubly linked in 2D grid)
    SectorNode* north;
    SectorNode* south;
    SectorNode* east;
    SectorNode* west;

    // Enemies in this sector
    Sentinel** enemies;
    int enemyCount;
    int maxEnemies;

    SectorNode(const std::string& name, const std::string& desc, int danger);
    ~SectorNode();

    // Enemy management
    void addEnemy(Sentinel* enemy);
    void removeEnemy(int index);
    void clearEnemies();
    bool hasEnemies() const;
};

/*******************************************************************************
 * DOUBLY LINKED LIST IMPLEMENTATION - GridMap
 *
 * Operations:
 * - moveNorth/South/East/West(): Navigate sectors - O(1)
 * - getCurrentSector(): Get current location - O(1)
 * - connectSectors(): Link two sectors - O(1)
 * - addSector(): Add new sector to map
 *
 * Structure (2D Grid):
 *          [Sector_N]
 *              |
 * [Sector_W]--[Current]--[Sector_E]
 *              |
 *          [Sector_S]
 *
 * Each sector can connect to 4 adjacent sectors via pointers.
 ******************************************************************************/
class GridMap {
private:
    SectorNode* currentSector;  // Player's current location
    SectorNode* startSector;    // Starting location (for reference)
    SectorNode** allSectors;    // Array of all sectors
    int sectorCount;            // Total sectors created
    int maxSectors;             // Maximum capacity

public:
    // Constructor
    GridMap();

    // Destructor
    ~GridMap();

    // Navigation: Move in direction
    // Step 1: Check if direction pointer is not null
    // Step 2: Update currentSector to new location
    // Step 3: Mark new sector as visited
    // Step 4: Return success/failure
    bool moveNorth();
    bool moveSouth();
    bool moveEast();
    bool moveWest();

    // Sector Management
    // Add sector to map
    void addSector(SectorNode* sector);

    // Connect two sectors bidirectionally
    // Step 1: Set sector1's direction pointer to sector2
    // Step 2: Set sector2's opposite direction pointer to sector1
    static void connectSectors(SectorNode* sector1, SectorNode* sector2,
                               const std::string& direction);

    // Set starting/current position
    void setCurrentSector(SectorNode* sector);
    void setStartSector(SectorNode* sector);

    // Display current sector information
    void displayCurrentSector() const;

    // Display available directions
    void displayAvailableDirections() const;

    // Getters
    SectorNode* getCurrentSector() const;
    SectorNode* getStartSector() const;
    SectorNode* getSector(int index) const;
    int getSectorCount() const;

    // Initialize default game map
    void initializeDefaultMap();

    // Get sector by name
    SectorNode* findSector(const std::string& name) const;
};

#endif // LINKEDLIST_H