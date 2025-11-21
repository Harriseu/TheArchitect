/*******************************************************************************
 * HashTable.cpp - Hash Table Implementation
 *
 * CONCEPT: Hash Tables (Composite Data Structure)
 *
 * Author: Digital Exodus Project
 * Course: COS30008 Data Structures and Patterns
 ******************************************************************************/

#include "../include/HashTable.h"

/*******************************************************************************
 * ITEM STRUCTURE IMPLEMENTATION
 ******************************************************************************/

Item::Item() : code(""), name(""), description(""), type(Type::CONSUMABLE),
               value(0), quantity(1) {
}

Item::Item(const std::string& code, const std::string& name,
           const std::string& desc, Type type, int value)
    : code(code), name(name), description(desc), type(type),
      value(value), quantity(1) {
}

/*******************************************************************************
 * HASH NODE IMPLEMENTATION
 ******************************************************************************/

HashNode::HashNode(const std::string& key, Item* item)
    : key(key), item(item), next(nullptr) {
}

HashNode::~HashNode() {
    // Note: Item deletion is handled by HashTable destructor
}

/*******************************************************************************
 * HASH TABLE IMPLEMENTATION
 *
 * Uses separate chaining for collision resolution.
 * djb2 hash function for string keys.
 ******************************************************************************/

// Constructor: Initialize empty buckets
HashTable::HashTable() : itemCount(0) {
    // Allocate array of bucket pointers
    buckets = new HashNode*[TABLE_SIZE];

    // Initialize all buckets to nullptr
    for (int i = 0; i < TABLE_SIZE; i++) {
        buckets[i] = nullptr;
    }
}

// Destructor: Free all nodes and items
HashTable::~HashTable() {
    // Iterate through each bucket
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode* current = buckets[i];

        // Delete all nodes in the chain
        while (current != nullptr) {
            HashNode* temp = current;
            current = current->next;

            // Delete the item
            if (temp->item != nullptr) {
                delete temp->item;
            }

            // Delete the node
            delete temp;
        }
    }

    // Delete the bucket array
    delete[] buckets;
}

// HASH FUNCTION: djb2 algorithm
// Step 1: Initialize hash to 5381 (magic number with good properties)
// Step 2: For each character: hash = hash * 33 + character
// Step 3: Return hash modulo table size
unsigned int HashTable::hashFunction(const std::string& key) const {
    unsigned int hash = 5381;  // Initial value

    // Process each character
    for (size_t i = 0; i < key.length(); i++) {
        // hash * 33 + c (using bit shift: 33 = 32 + 1)
        hash = ((hash << 5) + hash) + static_cast<unsigned int>(key[i]);
    }

    // Return index within table bounds
    return hash % TABLE_SIZE;
}

// INSERT: Add item to hash table
// Step 1: Calculate hash index from key
// Step 2: Create new HashNode
// Step 3: Insert at head of bucket's chain
void HashTable::insert(const std::string& key, Item* item) {
    // Step 1: Calculate the bucket index
    unsigned int index = hashFunction(key);

    // Step 2: Create new node
    HashNode* newNode = new HashNode(key, item);

    // Step 3: Insert at head of chain (O(1) insertion)
    newNode->next = buckets[index];
    buckets[index] = newNode;

    // Increment count
    itemCount++;

    std::cout << "[HashTable] Inserted '" << key << "' at bucket " << index << std::endl;
}

// GET: Retrieve item by key
// Step 1: Calculate hash index
// Step 2: Traverse chain at bucket
// Step 3: Return item if found, nullptr otherwise
Item* HashTable::get(const std::string& key) const {
    // Step 1: Calculate the bucket index
    unsigned int index = hashFunction(key);

    // Step 2: Traverse the chain
    HashNode* current = buckets[index];
    while (current != nullptr) {
        // Check if key matches
        if (current->key == key) {
            // Step 3: Found - return item
            return current->item;
        }
        current = current->next;
    }

    // Not found
    return nullptr;
}

// REMOVE: Delete item from table
// Step 1: Calculate hash index
// Step 2: Find node in chain
// Step 3: Update pointers and delete node
bool HashTable::remove(const std::string& key) {
    // Step 1: Calculate the bucket index
    unsigned int index = hashFunction(key);

    HashNode* current = buckets[index];
    HashNode* previous = nullptr;

    // Step 2: Find the node
    while (current != nullptr) {
        if (current->key == key) {
            // Step 3: Update pointers
            if (previous == nullptr) {
                // Node is at head of chain
                buckets[index] = current->next;
            } else {
                // Node is in middle or end
                previous->next = current->next;
            }

            // Delete item and node
            delete current->item;
            delete current;
            itemCount--;

            std::cout << "[HashTable] Removed '" << key << "'" << std::endl;
            return true;
        }

        previous = current;
        current = current->next;
    }

    // Not found
    return false;
}

// CONTAINS: Check if key exists
bool HashTable::contains(const std::string& key) const {
    return get(key) != nullptr;
}

// Utility functions
int HashTable::getSize() const {
    return itemCount;
}

bool HashTable::isEmpty() const {
    return itemCount == 0;
}

// Display all items in the hash table
void HashTable::displayAll() const {
    std::cout << "\n=== HASH TABLE CONTENTS ===" << std::endl;
    std::cout << "Total items: " << itemCount << std::endl;

    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode* current = buckets[i];
        if (current != nullptr) {
            std::cout << "Bucket " << i << ": ";
            while (current != nullptr) {
                std::cout << "[" << current->key << ": " << current->item->name << "] ";
                current = current->next;
            }
            std::cout << std::endl;
        }
    }
    std::cout << "===========================" << std::endl;
}

// Get all items as array
Item** HashTable::getAllItems(int& outCount) const {
    if (itemCount == 0) {
        outCount = 0;
        return nullptr;
    }

    Item** items = new Item*[itemCount];
    int index = 0;

    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode* current = buckets[i];
        while (current != nullptr) {
            items[index++] = current->item;
            current = current->next;
        }
    }

    outCount = itemCount;
    return items;
}

/*******************************************************************************
 * ITEM DATABASE IMPLEMENTATION
 ******************************************************************************/

ItemDatabase::ItemDatabase() {
    table = new HashTable();
}

ItemDatabase::~ItemDatabase() {
    delete table;
}

// Initialize with default game items
void ItemDatabase::initializeDefaultItems() {
    // Weapons
    addItem(createWeapon("WEAPON_PULSE", "Pulse Rifle",
            "Standard energy weapon for combat", 15));
    addItem(createWeapon("WEAPON_BLADE", "Code Blade",
            "Melee weapon that disrupts digital entities", 20));
    addItem(createWeapon("WEAPON_DISRUPTOR", "System Disruptor",
            "High-powered weapon for elite targets", 30));

    // Consumables
    addItem(createConsumable("HEAL_SMALL", "Data Fragment",
            "Restores 25 health points", 25));
    addItem(createConsumable("HEAL_MEDIUM", "Memory Cluster",
            "Restores 50 health points", 50));
    addItem(createConsumable("HEAL_LARGE", "Core Backup",
            "Restores 100 health points", 100));

    // Hack Tools
    addItem(createHackTool("HACK_BASIC", "Basic Decoder",
            "Bypasses simple security protocols", 10));
    addItem(createHackTool("HACK_ADVANCED", "Neural Bypass",
            "Advanced hacking tool for secure systems", 25));
    addItem(createHackTool("HACK_ELITE", "Quantum Key",
            "Unlocks the most secure terminals", 50));

    std::cout << "[ItemDatabase] Initialized with default items." << std::endl;
}

// Item operations
void ItemDatabase::addItem(Item* item) {
    if (item != nullptr) {
        table->insert(item->code, item);
    }
}

Item* ItemDatabase::getItem(const std::string& code) const {
    return table->get(code);
}

bool ItemDatabase::hasItem(const std::string& code) const {
    return table->contains(code);
}

void ItemDatabase::displayAllItems() const {
    table->displayAll();
}

// Factory methods
Item* ItemDatabase::createWeapon(const std::string& code, const std::string& name,
                                  const std::string& desc, int damage) {
    return new Item(code, name, desc, Item::Type::WEAPON, damage);
}

Item* ItemDatabase::createConsumable(const std::string& code, const std::string& name,
                                      const std::string& desc, int healAmount) {
    return new Item(code, name, desc, Item::Type::CONSUMABLE, healAmount);
}

Item* ItemDatabase::createHackTool(const std::string& code, const std::string& name,
                                    const std::string& desc, int power) {
    return new Item(code, name, desc, Item::Type::HACK_TOOL, power);
}
