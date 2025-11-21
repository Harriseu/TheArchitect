/*******************************************************************************
 * Iterator.cpp - Iterator Pattern Implementation
 *
 * CONCEPT: Iterator (Design Pattern)
 *
 * Author: Alif Harriz Jeffery | 102782711
 * Course: COS30008 Data Structures and Patterns
 ******************************************************************************/

#include "../include/Iterator.h"

/*******************************************************************************
 * INVENTORY IMPLEMENTATION
 ******************************************************************************/

Inventory::Inventory(int capacity)
    : head(nullptr), itemCount(0), maxCapacity(capacity) {
}

Inventory::~Inventory() {
    // Delete all nodes
    InventoryNode* current = head;
    while (current != nullptr) {
        InventoryNode* temp = current;
        current = current->next;
        // Note: Items are owned by inventory, delete them
        delete temp->item;
        delete temp;
    }
}

// Add item to inventory
bool Inventory::addItem(Item* item) {
    if (item == nullptr) return false;

    // Check if item already exists (stack it)
    if (hasItem(item->code)) {
        return increaseQuantity(item->code, item->quantity);
    }

    // Check capacity
    if (isFull()) {
        std::cout << "Inventory full! Cannot add " << item->name << std::endl;
        return false;
    }

    // Create new node and add at head
    InventoryNode* newNode = new InventoryNode(item);
    newNode->next = head;
    head = newNode;
    itemCount++;

    std::cout << "Added to inventory: " << item->name << std::endl;
    return true;
}

// Remove item by code
bool Inventory::removeItem(const std::string& code) {
    InventoryNode* current = head;
    InventoryNode* previous = nullptr;

    while (current != nullptr) {
        if (current->item->code == code) {
            // Unlink node
            if (previous == nullptr) {
                head = current->next;
            } else {
                previous->next = current->next;
            }

            std::cout << "Removed from inventory: " << current->item->name << std::endl;

            delete current->item;
            delete current;
            itemCount--;
            return true;
        }

        previous = current;
        current = current->next;
    }

    return false;
}

// Get item by code
Item* Inventory::getItem(const std::string& code) const {
    InventoryNode* current = head;
    while (current != nullptr) {
        if (current->item->code == code) {
            return current->item;
        }
        current = current->next;
    }
    return nullptr;
}

// Check if item exists
bool Inventory::hasItem(const std::string& code) const {
    return getItem(code) != nullptr;
}

// Increase item quantity
bool Inventory::increaseQuantity(const std::string& code, int amount) {
    Item* item = getItem(code);
    if (item != nullptr) {
        item->quantity += amount;
        std::cout << item->name << " x" << item->quantity << std::endl;
        return true;
    }
    return false;
}

// Decrease item quantity (remove if 0)
bool Inventory::decreaseQuantity(const std::string& code, int amount) {
    Item* item = getItem(code);
    if (item != nullptr) {
        item->quantity -= amount;
        if (item->quantity <= 0) {
            removeItem(code);
        }
        return true;
    }
    return false;
}

// Status methods
int Inventory::getItemCount() const { return itemCount; }
int Inventory::getMaxCapacity() const { return maxCapacity; }
bool Inventory::isFull() const { return itemCount >= maxCapacity; }
bool Inventory::isEmpty() const { return head == nullptr; }

// Display all items
void Inventory::displayAll() const {
    std::cout << "\n=== INVENTORY ===" << std::endl;
    std::cout << "Items: " << itemCount << "/" << maxCapacity << std::endl;
    std::cout << "-----------------" << std::endl;

    if (isEmpty()) {
        std::cout << "Inventory is empty." << std::endl;
    } else {
        InventoryNode* current = head;
        int index = 1;
        while (current != nullptr) {
            Item* item = current->item;
            std::cout << index++ << ". " << item->name;
            if (item->quantity > 1) {
                std::cout << " x" << item->quantity;
            }
            std::cout << " [" << item->code << "]" << std::endl;
            std::cout << "   " << item->description << std::endl;
            current = current->next;
        }
    }

    std::cout << "=================" << std::endl;
}

// ITERATOR FACTORY METHOD: Create iterator based on filter type
InventoryIterator* Inventory::createIterator(const std::string& filter) const {
    return new InventoryIterator(this, filter);
}

// Get raw head for iterator
InventoryNode* Inventory::getHead() const {
    return head;
}

/*******************************************************************************
 * INVENTORY ITERATOR IMPLEMENTATION
 *
 * Demonstrates Iterator Pattern for traversing inventory with optional filtering.
 ******************************************************************************/

// Constructor
// Step 1: Store reference to inventory
// Step 2: Set filter type
// Step 3: Position at first valid item
InventoryIterator::InventoryIterator(const Inventory* inv, const std::string& filter)
    : inventory(inv), currentNode(nullptr), filter(filter), hasFilter(false) {

    // Step 2: Determine filter type
    if (filter == "weapons") {
        filterType = Item::Type::WEAPON;
        hasFilter = true;
    } else if (filter == "consumables") {
        filterType = Item::Type::CONSUMABLE;
        hasFilter = true;
    } else if (filter == "hack_tools") {
        filterType = Item::Type::HACK_TOOL;
        hasFilter = true;
    } else if (filter == "key_items") {
        filterType = Item::Type::KEY_ITEM;
        hasFilter = true;
    } else {
        hasFilter = false;  // "all" or invalid filter
    }

    // Step 3: Position at first valid item
    reset();
}

// Destructor
InventoryIterator::~InventoryIterator() {
    // Iterator doesn't own the inventory
}

// Helper: Check if item matches filter
bool InventoryIterator::matchesFilter(Item* item) const {
    if (!hasFilter) return true;  // No filter, all match
    return item->type == filterType;
}

// Helper: Advance to next valid item
void InventoryIterator::advanceToNextValid() {
    while (currentNode != nullptr && !matchesFilter(currentNode->item)) {
        currentNode = currentNode->next;
    }
}

// HAS NEXT: Check if more items exist
// Step 1: Check if currentNode is not null
// Step 2: If filtering, verify current matches filter
bool InventoryIterator::hasNext() const {
    return currentNode != nullptr;
}

// NEXT: Return current item and advance
// Step 1: Store current item
// Step 2: Move to next node
// Step 3: If filtering, skip non-matching items
// Step 4: Return stored item
Item* InventoryIterator::next() {
    if (!hasNext()) return nullptr;

    // Step 1: Store current
    Item* result = currentNode->item;

    // Step 2: Move to next
    currentNode = currentNode->next;

    // Step 3: Skip non-matching (if filtering)
    advanceToNextValid();

    // Step 4: Return
    return result;
}

// RESET: Return to beginning of iteration
// Step 1: Set currentNode to inventory head
// Step 2: If filtering, advance to first matching item
void InventoryIterator::reset() {
    // Step 1: Reset to head
    currentNode = inventory->getHead();

    // Step 2: Find first matching item
    advanceToNextValid();
}

// Get current item without advancing
Item* InventoryIterator::current() const {
    if (currentNode != nullptr) {
        return currentNode->item;
    }
    return nullptr;
}

/*******************************************************************************
 * SORTED INVENTORY ITERATOR IMPLEMENTATION
 ******************************************************************************/

SortedInventoryIterator::SortedInventoryIterator(const Inventory* inv, SortBy sortBy)
    : sortedItems(nullptr), totalItems(0), currentIndex(0) {

    // Count items
    InventoryNode* node = inv->getHead();
    while (node != nullptr) {
        totalItems++;
        node = node->next;
    }

    if (totalItems == 0) return;

    // Create array of item pointers
    sortedItems = new Item*[totalItems];
    node = inv->getHead();
    int i = 0;
    while (node != nullptr) {
        sortedItems[i++] = node->item;
        node = node->next;
    }

    // Sort based on criteria (bubble sort)
    for (int i = 0; i < totalItems - 1; i++) {
        for (int j = 0; j < totalItems - i - 1; j++) {
            bool shouldSwap = false;

            switch (sortBy) {
                case SortBy::NAME:
                    shouldSwap = sortedItems[j]->name > sortedItems[j + 1]->name;
                    break;
                case SortBy::VALUE:
                    shouldSwap = sortedItems[j]->value < sortedItems[j + 1]->value;
                    break;
                case SortBy::TYPE:
                    shouldSwap = static_cast<int>(sortedItems[j]->type) >
                                 static_cast<int>(sortedItems[j + 1]->type);
                    break;
            }

            if (shouldSwap) {
                Item* temp = sortedItems[j];
                sortedItems[j] = sortedItems[j + 1];
                sortedItems[j + 1] = temp;
            }
        }
    }
}

SortedInventoryIterator::~SortedInventoryIterator() {
    delete[] sortedItems;
}

bool SortedInventoryIterator::hasNext() const {
    return currentIndex < totalItems;
}

Item* SortedInventoryIterator::next() {
    if (!hasNext()) return nullptr;
    return sortedItems[currentIndex++];
}

void SortedInventoryIterator::reset() {
    currentIndex = 0;
}

Item* SortedInventoryIterator::current() const {
    if (currentIndex < totalItems) {
        return sortedItems[currentIndex];
    }
    return nullptr;
}