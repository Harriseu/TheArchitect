/*******************************************************************************
 * Iterator.h - Iterator Pattern for Inventory Traversal
 *
 * CONCEPT: Iterator (Design Pattern)
 *
 * This iterator provides a way to traverse the player's inventory
 * without exposing the underlying data structure.
 * Supports filtered iteration (by item type) and multiple strategies.
 *
 * WHY ITERATOR:
 * - Decouples inventory structure from traversal logic
 * - Allows multiple simultaneous iterations
 * - Supports different traversal strategies (all, filtered, sorted)
 * - Client code doesn't need to know internal structure
 *
 * Author: Digital Exodus Project
 * Course: COS30008 Data Structures and Patterns
 ******************************************************************************/

#ifndef ITERATOR_H
#define ITERATOR_H

#include <string>
#include "HashTable.h"

/*******************************************************************************
 * CONCEPT: ITERATOR PATTERN - Abstract Iterator Interface
 *
 * Defines the interface for all iterators.
 * Concrete iterators implement these methods.
 ******************************************************************************/
template<typename T>
class Iterator {
public:
    virtual ~Iterator() {}

    // hasNext: Check if more elements exist
    virtual bool hasNext() const = 0;

    // next: Return current element and advance
    virtual T next() = 0;

    // reset: Return to beginning
    virtual void reset() = 0;

    // current: Get current element without advancing
    virtual T current() const = 0;
};

/*******************************************************************************
 * Inventory Item Node - For internal linked storage
 ******************************************************************************/
struct InventoryNode {
    Item* item;
    InventoryNode* next;

    InventoryNode(Item* item) : item(item), next(nullptr) {}
};

/*******************************************************************************
 * Inventory Class - Aggregate that creates iterators
 *
 * Stores player's items and provides iterator creation.
 ******************************************************************************/
class Inventory {
private:
    InventoryNode* head;        // Head of item list
    int itemCount;              // Total items
    int maxCapacity;            // Maximum items allowed

public:
    Inventory(int capacity = 50);
    ~Inventory();

    // Item management
    bool addItem(Item* item);
    bool removeItem(const std::string& code);
    Item* getItem(const std::string& code) const;
    bool hasItem(const std::string& code) const;

    // Quantity management
    bool increaseQuantity(const std::string& code, int amount);
    bool decreaseQuantity(const std::string& code, int amount);

    // Status
    int getItemCount() const;
    int getMaxCapacity() const;
    bool isFull() const;
    bool isEmpty() const;

    // Display
    void displayAll() const;

    // Iterator creation (Factory Method pattern)
    // Creates iterator based on filter type
    class InventoryIterator* createIterator(const std::string& filter = "all") const;

    // Get raw access for iterator
    InventoryNode* getHead() const;
};

/*******************************************************************************
 * CONCEPT: ITERATOR PATTERN - Concrete Iterator Implementation
 *
 * InventoryIterator traverses inventory items with optional filtering.
 *
 * Usage:
 *   InventoryIterator* it = inventory.createIterator("weapons");
 *   while (it->hasNext()) {
 *       Item* item = it->next();
 *       // Process item
 *   }
 *   delete it;
 *
 * Filter options:
 * - "all": All items
 * - "weapons": Only weapons
 * - "consumables": Only consumables
 * - "hack_tools": Only hacking tools
 * - "key_items": Only key items
 ******************************************************************************/
class InventoryIterator : public Iterator<Item*> {
private:
    const Inventory* inventory; // Reference to inventory
    InventoryNode* currentNode; // Current position
    std::string filter;         // Filter type
    Item::Type filterType;      // Converted filter type
    bool hasFilter;             // Whether filtering is active

    // Helper: Move to next valid item (respecting filter)
    void advanceToNextValid();

    // Helper: Check if item matches filter
    bool matchesFilter(Item* item) const;

public:
    // Constructor
    // Step 1: Store reference to inventory
    // Step 2: Set filter type
    // Step 3: Position at first valid item
    InventoryIterator(const Inventory* inv, const std::string& filter);

    // Destructor
    ~InventoryIterator() override;

    // hasNext: Check if more items exist
    // Step 1: Check if currentNode is not null
    // Step 2: If filtering, verify current matches filter
    bool hasNext() const override;

    // next: Return current item and advance
    // Step 1: Store current item
    // Step 2: Move to next node
    // Step 3: If filtering, skip non-matching items
    // Step 4: Return stored item
    Item* next() override;

    // reset: Return to beginning of iteration
    // Step 1: Set currentNode to inventory head
    // Step 2: If filtering, advance to first matching item
    void reset() override;

    // current: Get current item without advancing
    Item* current() const override;
};

/*******************************************************************************
 * Sorted Inventory Iterator - Iterates in sorted order
 *
 * Creates a sorted copy for iteration (by name or value).
 ******************************************************************************/
class SortedInventoryIterator : public Iterator<Item*> {
private:
    Item** sortedItems;         // Array of sorted item pointers
    int totalItems;             // Number of items
    int currentIndex;           // Current position

public:
    enum class SortBy {
        NAME,
        VALUE,
        TYPE
    };

    // Constructor - creates sorted copy
    SortedInventoryIterator(const Inventory* inv, SortBy sortBy);

    // Destructor
    ~SortedInventoryIterator() override;

    // Iterator interface
    bool hasNext() const override;
    Item* next() override;
    void reset() override;
    Item* current() const override;
};

#endif // ITERATOR_H
