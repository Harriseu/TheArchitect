/*******************************************************************************
 * HashTable.h - Hash Table Implementation for Item Database
 *
 * CONCEPT: Hash Tables (Composite Data Structure)
 *
 * This hash table provides O(1) average-case lookup for game items.
 * Items are stored using string keys (item codes like "HACK_001").
 * Collision handling is done via chaining (linked list at each bucket).
 *
 * WHY HASH TABLE:
 * - Fast lookup crucial for real-time inventory access
 * - Player needs instant item retrieval during combat
 * - O(1) average vs O(n) for array or O(log n) for BST
 *
 * Author: Alif Harriz Jeffery | 102782711
 * Course: COS30008 Data Structures and Patterns
 ******************************************************************************/

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <iostream>

/*******************************************************************************
 * Item Structure - Stored in Hash Table
 ******************************************************************************/
struct Item {
    std::string code;           // Unique identifier (e.g., "WEAPON_001")
    std::string name;           // Display name
    std::string description;    // Item description

    enum class Type {
        WEAPON,
        CONSUMABLE,
        KEY_ITEM,
        HACK_TOOL
    } type;

    int value;                  // Power/healing amount/etc
    int quantity;               // Stack count

    Item();
    Item(const std::string& code, const std::string& name,
         const std::string& desc, Type type, int value);
};

/*******************************************************************************
 * CONCEPT: HASH TABLE - Node for Chaining
 *
 * Each bucket contains a linked list of HashNodes to handle collisions.
 * When two keys hash to the same index, they are chained together.
 ******************************************************************************/
struct HashNode {
    std::string key;            // The item code (hash key)
    Item* item;                 // Pointer to the item
    HashNode* next;             // Next node in chain (collision handling)

    HashNode(const std::string& key, Item* item);
    ~HashNode();
};

/*******************************************************************************
 * CONCEPT: HASH TABLE IMPLEMENTATION
 *
 * Operations:
 * - insert(key, item): Add item to table - O(1) average
 * - get(key): Retrieve item by code - O(1) average
 * - remove(key): Delete item - O(1) average
 * - contains(key): Check existence - O(1) average
 *
 * Hash Function: djb2 algorithm (efficient string hashing)
 * Collision Resolution: Separate chaining with linked lists
 ******************************************************************************/
class HashTable {
private:
    static const int TABLE_SIZE = 53;   // Prime number for better distribution
    HashNode** buckets;                  // Array of bucket pointers
    int itemCount;                       // Number of items stored

    // djb2 hash function - excellent distribution for strings
    // Step 1: Initialize hash to 5381 (magic number with good properties)
    // Step 2: For each character: hash = hash * 33 + character
    // Step 3: Return hash modulo table size
    unsigned int hashFunction(const std::string& key) const;

public:
    // Constructor - initializes empty buckets
    HashTable();

    // Destructor - frees all nodes and items
    ~HashTable();

    // Core Operations

    // Insert: Add item to hash table
    // Step 1: Calculate hash index from key
    // Step 2: Create new HashNode
    // Step 3: Insert at head of bucket's chain
    void insert(const std::string& key, Item* item);

    // Get: Retrieve item by key
    // Step 1: Calculate hash index
    // Step 2: Traverse chain at bucket
    // Step 3: Return item if found, nullptr otherwise
    Item* get(const std::string& key) const;

    // Remove: Delete item from table
    // Step 1: Calculate hash index
    // Step 2: Find node in chain
    // Step 3: Update pointers and delete node
    bool remove(const std::string& key);

    // Contains: Check if key exists
    bool contains(const std::string& key) const;

    // Utility
    int getSize() const;
    bool isEmpty() const;
    void displayAll() const;

    // Get all items as array (for iteration)
    Item** getAllItems(int& outCount) const;
};

/*******************************************************************************
 * ItemDatabase - Wrapper for game item management
 *
 * Uses HashTable internally for fast item lookups.
 * Provides game-specific item creation and management.
 ******************************************************************************/
class ItemDatabase {
private:
    HashTable* table;

public:
    ItemDatabase();
    ~ItemDatabase();

    // Initialize with default game items
    void initializeDefaultItems();

    // Item operations
    void addItem(Item* item);
    Item* getItem(const std::string& code) const;
    bool hasItem(const std::string& code) const;
    void displayAllItems() const;

    // Factory methods for creating specific item types
    static Item* createWeapon(const std::string& code, const std::string& name,
                              const std::string& desc, int damage);
    static Item* createConsumable(const std::string& code, const std::string& name,
                                  const std::string& desc, int healAmount);
    static Item* createHackTool(const std::string& code, const std::string& name,
                                const std::string& desc, int power);
};

#endif // HASHTABLE_H