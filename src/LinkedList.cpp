/*******************************************************************************
 * LinkedList.cpp - Singly and Doubly Linked List Implementations
 *
 * CONCEPT: Singly Linked List (Event Log), Doubly Linked List (Sector Navigation)
 *
 * Author: Alif Harriz Jeffery | 102782711
 * Course: COS30008 Data Structures and Patterns
 ******************************************************************************/

#include "../include/LinkedList.h"
#include "../include/Entity.h"
#include <ctime>
#include <sstream>

/*******************************************************************************
 * EVENT NODE IMPLEMENTATION (Singly Linked List)
 ******************************************************************************/

EventNode::EventNode(const std::string& desc, const std::string& type)
    : description(desc), eventType(type), next(nullptr) {
    // Generate timestamp
    time_t now = time(nullptr);
    char buffer[26];
    SAFE_CTIME(&now, buffer, sizeof(buffer));
    timestamp = std::string(buffer);
    // Remove newline from ctime output
    if (!timestamp.empty() && timestamp.back() == '\n') {
        timestamp.pop_back();
    }
}

/*******************************************************************************
 * EVENT LOG IMPLEMENTATION (Singly Linked List)
 *
 * Structure: HEAD -> [Event3] -> [Event2] -> [Event1] -> nullptr
 *            (newest)                           (oldest)
 ******************************************************************************/

// Constructor - Initialize empty log
EventLog::EventLog() : head(nullptr), eventCount(0) {
}

// Destructor - Free all nodes
EventLog::~EventLog() {
    clear();
}

// ADD EVENT: Insert new event at head
// Step 1: Create new EventNode with description and type
// Step 2: Set new node's next pointer to current head
// Step 3: Update head to point to new node
// Step 4: Increment event count
void EventLog::addEvent(const std::string& description, const std::string& type) {
    // Step 1: Create new node
    EventNode* newNode = new EventNode(description, type);

    // Step 2: Link to current head
    newNode->next = head;

    // Step 3: Update head
    head = newNode;

    // Step 4: Increment count
    eventCount++;

    std::cout << "[EventLog] Added: [" << type << "] " << description << std::endl;
}

// DISPLAY RECENT: Show last n events
// Step 1: Start at head (most recent)
// Step 2: Traverse and display up to n events
// Step 3: Stop at nullptr or after n events
void EventLog::displayRecent(int count) const {
    std::cout << "\n=== RECENT EVENTS ===" << std::endl;

    if (isEmpty()) {
        std::cout << "No events recorded." << std::endl;
        return;
    }

    // Step 1: Start at head
    EventNode* current = head;
    int displayed = 0;

    // Step 2 & 3: Traverse and display
    while (current != nullptr && displayed < count) {
        std::cout << "[" << current->eventType << "] "
                  << current->description << std::endl;
        std::cout << "  Time: " << current->timestamp << std::endl;

        current = current->next;
        displayed++;
    }

    std::cout << "=====================" << std::endl;
}

// Display all events
void EventLog::displayAll() const {
    std::cout << "\n=== COMPLETE EVENT LOG ===" << std::endl;
    std::cout << "Total events: " << eventCount << std::endl;

    if (isEmpty()) {
        std::cout << "No events recorded." << std::endl;
        return;
    }

    EventNode* current = head;
    int index = 1;

    while (current != nullptr) {
        std::cout << index++ << ". [" << current->eventType << "] "
                  << current->description << std::endl;
        current = current->next;
    }

    std::cout << "==========================" << std::endl;
}

// CLEAR: Remove all events
// Step 1: Traverse list from head
// Step 2: Delete each node
// Step 3: Set head to nullptr, count to 0
void EventLog::clear() {
    // Step 1 & 2: Traverse and delete
    EventNode* current = head;
    while (current != nullptr) {
        EventNode* temp = current;
        current = current->next;
        delete temp;
    }

    // Step 3: Reset
    head = nullptr;
    eventCount = 0;
}

// Getters
int EventLog::getEventCount() const {
    return eventCount;
}

bool EventLog::isEmpty() const {
    return head == nullptr;
}

// Get events as string array (for save system)
std::string* EventLog::getEventsAsArray(int& outCount) const {
    if (isEmpty()) {
        outCount = 0;
        return nullptr;
    }

    std::string* events = new std::string[eventCount];
    EventNode* current = head;
    int index = 0;

    while (current != nullptr) {
        events[index++] = current->eventType + "|" + current->description;
        current = current->next;
    }

    outCount = eventCount;
    return events;
}

/*******************************************************************************
 * SECTOR NODE IMPLEMENTATION (Doubly Linked List)
 ******************************************************************************/

SectorNode::SectorNode(const std::string& name, const std::string& desc, int danger)
    : name(name), description(desc), dangerLevel(danger),
      visited(false), hasTerminal(false),
      north(nullptr), south(nullptr), east(nullptr), west(nullptr),
      enemies(nullptr), enemyCount(0), maxEnemies(5) {

    // Allocate enemy array
    enemies = new Sentinel*[maxEnemies];
    for (int i = 0; i < maxEnemies; i++) {
        enemies[i] = nullptr;
    }
}

SectorNode::~SectorNode() {
    clearEnemies();
    delete[] enemies;
}

// Add enemy to sector
void SectorNode::addEnemy(Sentinel* enemy) {
    if (enemyCount < maxEnemies && enemy != nullptr) {
        enemies[enemyCount++] = enemy;
    }
}

// Remove enemy at index
void SectorNode::removeEnemy(int index) {
    if (index >= 0 && index < enemyCount) {
        delete enemies[index];

        // Shift remaining enemies
        for (int i = index; i < enemyCount - 1; i++) {
            enemies[i] = enemies[i + 1];
        }
        enemies[enemyCount - 1] = nullptr;
        enemyCount--;
    }
}

// Clear all enemies
void SectorNode::clearEnemies() {
    for (int i = 0; i < enemyCount; i++) {
        delete enemies[i];
        enemies[i] = nullptr;
    }
    enemyCount = 0;
}

bool SectorNode::hasEnemies() const {
    return enemyCount > 0;
}

/*******************************************************************************
 * GRID MAP IMPLEMENTATION (Doubly Linked List - 4-directional)
 *
 * Structure (2D Grid):
 *          [Sector_N]
 *              |
 * [Sector_W]--[Current]--[Sector_E]
 *              |
 *          [Sector_S]
 ******************************************************************************/

// Constructor
GridMap::GridMap()
    : currentSector(nullptr), startSector(nullptr),
      allSectors(nullptr), sectorCount(0), maxSectors(50) {

    allSectors = new SectorNode*[maxSectors];
    for (int i = 0; i < maxSectors; i++) {
        allSectors[i] = nullptr;
    }
}

// Destructor
GridMap::~GridMap() {
    // Delete all sectors
    for (int i = 0; i < sectorCount; i++) {
        delete allSectors[i];
    }
    delete[] allSectors;
}

// NAVIGATION: Move in direction
// Step 1: Check if direction pointer is not null
// Step 2: Update currentSector to new location
// Step 3: Mark new sector as visited
// Step 4: Return success/failure
bool GridMap::moveNorth() {
    // Step 1: Check if north path exists
    if (currentSector == nullptr || currentSector->north == nullptr) {
        std::cout << "Cannot move north - no passage exists." << std::endl;
        return false;
    }

    // Step 2: Update current sector
    currentSector = currentSector->north;

    // Step 3: Mark as visited
    currentSector->visited = true;

    std::cout << "Moved NORTH to: " << currentSector->name << std::endl;
    return true;
}

bool GridMap::moveSouth() {
    if (currentSector == nullptr || currentSector->south == nullptr) {
        std::cout << "Cannot move south - no passage exists." << std::endl;
        return false;
    }

    currentSector = currentSector->south;
    currentSector->visited = true;

    std::cout << "Moved SOUTH to: " << currentSector->name << std::endl;
    return true;
}

bool GridMap::moveEast() {
    if (currentSector == nullptr || currentSector->east == nullptr) {
        std::cout << "Cannot move east - no passage exists." << std::endl;
        return false;
    }

    currentSector = currentSector->east;
    currentSector->visited = true;

    std::cout << "Moved EAST to: " << currentSector->name << std::endl;
    return true;
}

bool GridMap::moveWest() {
    if (currentSector == nullptr || currentSector->west == nullptr) {
        std::cout << "Cannot move west - no passage exists." << std::endl;
        return false;
    }

    currentSector = currentSector->west;
    currentSector->visited = true;

    std::cout << "Moved WEST to: " << currentSector->name << std::endl;
    return true;
}

// Add sector to map
void GridMap::addSector(SectorNode* sector) {
    if (sectorCount < maxSectors && sector != nullptr) {
        allSectors[sectorCount++] = sector;
    }
}

// CONNECT SECTORS: Link two sectors bidirectionally
// Step 1: Set sector1's direction pointer to sector2
// Step 2: Set sector2's opposite direction pointer to sector1
void GridMap::connectSectors(SectorNode* sector1, SectorNode* sector2,
                             const std::string& direction) {
    if (sector1 == nullptr || sector2 == nullptr) return;

    // Step 1 & 2: Create bidirectional link
    if (direction == "north") {
        sector1->north = sector2;
        sector2->south = sector1;
    } else if (direction == "south") {
        sector1->south = sector2;
        sector2->north = sector1;
    } else if (direction == "east") {
        sector1->east = sector2;
        sector2->west = sector1;
    } else if (direction == "west") {
        sector1->west = sector2;
        sector2->east = sector1;
    }

    std::cout << "[GridMap] Connected " << sector1->name << " <-> "
              << sector2->name << " (" << direction << ")" << std::endl;
}

void GridMap::setCurrentSector(SectorNode* sector) {
    currentSector = sector;
    if (currentSector != nullptr) {
        currentSector->visited = true;
    }
}

void GridMap::setStartSector(SectorNode* sector) {
    startSector = sector;
}

// Display current sector information
void GridMap::displayCurrentSector() const {
    if (currentSector == nullptr) {
        std::cout << "Location unknown." << std::endl;
        return;
    }

    std::cout << "\n========================================" << std::endl;
    std::cout << "  LOCATION: " << currentSector->name << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << currentSector->description << std::endl;
    std::cout << "Danger Level: ";
    for (int i = 0; i < currentSector->dangerLevel; i++) std::cout << "*";
    for (int i = currentSector->dangerLevel; i < 10; i++) std::cout << ".";
    std::cout << " (" << currentSector->dangerLevel << "/10)" << std::endl;

    if (currentSector->hasTerminal) {
        std::cout << "[!] Hackable terminal detected" << std::endl;
    }

    if (currentSector->hasEnemies()) {
        std::cout << "[!] Hostile entities present: " << currentSector->enemyCount << std::endl;
    }

    std::cout << "========================================" << std::endl;
}

// Display available directions
void GridMap::displayAvailableDirections() const {
    if (currentSector == nullptr) return;

    std::cout << "Available passages: ";
    bool hasPath = false;

    if (currentSector->north != nullptr) {
        std::cout << "[N]orth ";
        hasPath = true;
    }
    if (currentSector->south != nullptr) {
        std::cout << "[S]outh ";
        hasPath = true;
    }
    if (currentSector->east != nullptr) {
        std::cout << "[E]ast ";
        hasPath = true;
    }
    if (currentSector->west != nullptr) {
        std::cout << "[W]est ";
        hasPath = true;
    }

    if (!hasPath) {
        std::cout << "None - Dead end!";
    }
    std::cout << std::endl;
}

// Getters
SectorNode* GridMap::getCurrentSector() const { return currentSector; }
SectorNode* GridMap::getStartSector() const { return startSector; }
SectorNode* GridMap::getSector(int index) const {
    if (index >= 0 && index < sectorCount) {
        return allSectors[index];
    }
    return nullptr;
}
int GridMap::getSectorCount() const { return sectorCount; }

// Find sector by name
SectorNode* GridMap::findSector(const std::string& name) const {
    for (int i = 0; i < sectorCount; i++) {
        if (allSectors[i] != nullptr && allSectors[i]->name == name) {
            return allSectors[i];
        }
    }
    return nullptr;
}

// Initialize default game map
void GridMap::initializeDefaultMap() {
    // Create sectors
    SectorNode* spawn = new SectorNode("Data Stream Junction",
        "A central hub where data flows converge. Faint pulses of light stream through the corridors.", 1);
    spawn->hasTerminal = true;

    SectorNode* sector1 = new SectorNode("Firewall Array",
        "Massive defensive barriers shimmer in the darkness. Security protocols are active.", 3);

    SectorNode* sector2 = new SectorNode("Memory Banks",
        "Towering structures of stored data reach into the void. Ancient information waits to be accessed.", 2);
    sector2->hasTerminal = true;

    SectorNode* sector3 = new SectorNode("Processing Core",
        "The heart of local computation. Heat radiates from overworked systems.", 4);

    SectorNode* sector4 = new SectorNode("Archive Depths",
        "Forgotten data rests here. Corrupted files flicker at the edges of perception.", 3);
    sector4->hasTerminal = true;

    SectorNode* sector5 = new SectorNode("Sentinel Patrol Zone",
        "Warning: High security area. Sentinel activity detected at regular intervals.", 6);

    SectorNode* sector6 = new SectorNode("The Sanctuary Gate",
        "An ancient portal leads to the Awakened safe haven. The exit is near...", 5);
    sector6->hasTerminal = true;

    // Add sectors to map
    addSector(spawn);
    addSector(sector1);
    addSector(sector2);
    addSector(sector3);
    addSector(sector4);
    addSector(sector5);
    addSector(sector6);

    // Connect sectors (creating the doubly linked structure)
    connectSectors(spawn, sector1, "north");
    connectSectors(spawn, sector2, "east");
    connectSectors(sector1, sector3, "north");
    connectSectors(sector2, sector4, "east");
    connectSectors(sector3, sector5, "east");
    connectSectors(sector4, sector5, "north");
    connectSectors(sector5, sector6, "north");

    // Set starting position
    setStartSector(spawn);
    setCurrentSector(spawn);

    std::cout << "[GridMap] Default map initialized with " << sectorCount << " sectors." << std::endl;
}