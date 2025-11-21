/*******************************************************************************
 * Tree.cpp - Tree Implementation for Ability System
 *
 * CONCEPT: Tree (Abstract Data Type)
 *
 * Author: Alif Harriz Jeffery | 102782711
 * Course: COS30008 Data Structures and Patterns
 ******************************************************************************/

#include "../include/Tree.h"

/*******************************************************************************
 * ABILITY STRUCTURE IMPLEMENTATION
 ******************************************************************************/

Ability::Ability()
    : code(""), name(""), description(""), cost(0), power(0),
      unlocked(false), type(Type::COMBAT) {
}

Ability::Ability(const std::string& code, const std::string& name,
                 const std::string& desc, Type type, int cost, int power)
    : code(code), name(name), description(desc), type(type),
      cost(cost), power(power), unlocked(false) {
}

/*******************************************************************************
 * ABILITY NODE IMPLEMENTATION
 ******************************************************************************/

AbilityNode::AbilityNode(const Ability& ability)
    : ability(ability), left(nullptr), right(nullptr), parent(nullptr) {
}

AbilityNode::~AbilityNode() {
    // Node deletion is handled by tree destructor
}

/*******************************************************************************
 * ABILITY TREE IMPLEMENTATION
 *
 * Structure Example:
 *                    [Basic Combat] (root - always unlocked)
 *                    /            \
 *           [Time Dilation]    [Code Shield]
 *              /      \           /        \
 *        [Overdrive] [Stealth] [Firewall] [Regen]
 ******************************************************************************/

// Constructor
AbilityTree::AbilityTree()
    : root(nullptr), totalAbilities(0), unlockedCount(0), skillPoints(0) {
}

// Destructor
AbilityTree::~AbilityTree() {
    deleteTree(root);
}

// Helper: Recursively delete tree
void AbilityTree::deleteTree(AbilityNode* node) {
    if (node == nullptr) return;

    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

// Set root ability (always unlocked)
void AbilityTree::setRoot(const Ability& ability) {
    if (root != nullptr) {
        deleteTree(root);
    }

    root = new AbilityNode(ability);
    root->ability.unlocked = true;  // Root is always unlocked
    totalAbilities = 1;
    unlockedCount = 1;

    std::cout << "[AbilityTree] Root set: " << ability.name << std::endl;
}

// Helper: Find node by code (recursive search)
AbilityNode* AbilityTree::findNode(AbilityNode* node, const std::string& code) const {
    if (node == nullptr) return nullptr;

    // Check current node
    if (node->ability.code == code) {
        return node;
    }

    // Search left subtree
    AbilityNode* found = findNode(node->left, code);
    if (found != nullptr) return found;

    // Search right subtree
    return findNode(node->right, code);
}

// INSERT: Add ability as child of parent
// Step 1: Find parent node by code
// Step 2: Create new AbilityNode
// Step 3: Attach as left or right child
// Step 4: Set parent pointer
// Step 5: Increment total count
bool AbilityTree::insert(const Ability& ability, const std::string& parentCode, bool asLeftChild) {
    // Step 1: Find parent
    AbilityNode* parentNode = findNode(root, parentCode);
    if (parentNode == nullptr) {
        std::cout << "[AbilityTree] Parent not found: " << parentCode << std::endl;
        return false;
    }

    // Step 2: Create new node
    AbilityNode* newNode = new AbilityNode(ability);

    // Step 3 & 4: Attach to parent
    newNode->parent = parentNode;

    if (asLeftChild) {
        if (parentNode->left != nullptr) {
            std::cout << "[AbilityTree] Left child already exists!" << std::endl;
            delete newNode;
            return false;
        }
        parentNode->left = newNode;
    } else {
        if (parentNode->right != nullptr) {
            std::cout << "[AbilityTree] Right child already exists!" << std::endl;
            delete newNode;
            return false;
        }
        parentNode->right = newNode;
    }

    // Step 5: Increment count
    totalAbilities++;

    std::cout << "[AbilityTree] Added: " << ability.name << " under " << parentCode << std::endl;
    return true;
}

// FIND: Locate ability by code
Ability* AbilityTree::findAbility(const std::string& code) const {
    AbilityNode* node = findNode(root, code);
    if (node != nullptr) {
        return &(node->ability);
    }
    return nullptr;
}

// CAN UNLOCK: Check if ability can be unlocked
// Step 1: Find the ability node
// Step 2: Check if parent is unlocked (or is root)
// Step 3: Check if player has enough skill points
// Step 4: Check if not already unlocked
bool AbilityTree::canUnlock(const std::string& code) const {
    // Step 1: Find the node
    AbilityNode* node = findNode(root, code);
    if (node == nullptr) return false;

    // Step 4: Check if already unlocked
    if (node->ability.unlocked) {
        return false;
    }

    // Step 2: Check parent (root has no parent, always OK)
    if (node->parent != nullptr && !node->parent->ability.unlocked) {
        return false;  // Parent not unlocked
    }

    // Step 3: Check skill points
    if (skillPoints < node->ability.cost) {
        return false;  // Not enough points
    }

    return true;
}

// UNLOCK: Attempt to unlock an ability
// Step 1: Verify canUnlock returns true
// Step 2: Deduct skill points
// Step 3: Mark ability as unlocked
// Step 4: Increment unlocked count
bool AbilityTree::unlock(const std::string& code) {
    // Step 1: Check if can unlock
    if (!canUnlock(code)) {
        AbilityNode* node = findNode(root, code);
        if (node == nullptr) {
            std::cout << "Ability not found: " << code << std::endl;
        } else if (node->ability.unlocked) {
            std::cout << "Ability already unlocked: " << node->ability.name << std::endl;
        } else if (node->parent != nullptr && !node->parent->ability.unlocked) {
            std::cout << "Must unlock prerequisite first: " << node->parent->ability.name << std::endl;
        } else {
            std::cout << "Not enough skill points! Need: " << node->ability.cost
                      << ", Have: " << skillPoints << std::endl;
        }
        return false;
    }

    // Find the node
    AbilityNode* node = findNode(root, code);

    // Step 2: Deduct points
    skillPoints -= node->ability.cost;

    // Step 3: Mark as unlocked
    node->ability.unlocked = true;

    // Step 4: Increment count
    unlockedCount++;

    std::cout << "\n*** ABILITY UNLOCKED ***" << std::endl;
    std::cout << node->ability.name << ": " << node->ability.description << std::endl;
    std::cout << "Remaining skill points: " << skillPoints << std::endl;

    return true;
}

// Helper: Display node with indentation
void AbilityTree::displayNode(AbilityNode* node, int depth) const {
    if (node == nullptr) return;

    // Print indentation
    for (int i = 0; i < depth; i++) {
        std::cout << "  ";
    }

    // Print node info
    std::cout << (node->ability.unlocked ? "[X] " : "[ ] ");
    std::cout << node->ability.name;
    std::cout << " (Cost: " << node->ability.cost << ", Power: " << node->ability.power << ")";

    if (!node->ability.unlocked && canUnlock(node->ability.code)) {
        std::cout << " <-- CAN UNLOCK";
    }
    std::cout << std::endl;

    // Recursively display children
    displayNode(node->left, depth + 1);
    displayNode(node->right, depth + 1);
}

// Display entire tree
void AbilityTree::displayTree() const {
    std::cout << "\n=== ABILITY TREE ===" << std::endl;
    std::cout << "Skill Points: " << skillPoints << std::endl;
    std::cout << "Unlocked: " << unlockedCount << "/" << totalAbilities << std::endl;
    std::cout << "-------------------" << std::endl;

    if (root == nullptr) {
        std::cout << "No abilities available." << std::endl;
    } else {
        displayNode(root, 0);
    }

    std::cout << "===================" << std::endl;
}

// Display available abilities (can be unlocked now)
void AbilityTree::displayAvailable() const {
    std::cout << "\n=== AVAILABLE ABILITIES ===" << std::endl;
    std::cout << "Skill Points: " << skillPoints << std::endl;

    int count = 0;
    Ability** available = getAvailableAbilities(count);

    if (count == 0) {
        std::cout << "No abilities available for unlocking." << std::endl;
    } else {
        for (int i = 0; i < count; i++) {
            std::cout << (i + 1) << ". " << available[i]->name
                      << " (Cost: " << available[i]->cost << ")" << std::endl;
            std::cout << "   " << available[i]->description << std::endl;
        }
    }

    delete[] available;
    std::cout << "===========================" << std::endl;
}

// Display unlocked abilities
void AbilityTree::displayUnlocked() const {
    std::cout << "\n=== UNLOCKED ABILITIES ===" << std::endl;

    int count = 0;
    Ability** unlocked = getUnlockedAbilities(count);

    if (count == 0) {
        std::cout << "No abilities unlocked." << std::endl;
    } else {
        for (int i = 0; i < count; i++) {
            std::cout << (i + 1) << ". " << unlocked[i]->name
                      << " (Power: " << unlocked[i]->power << ")" << std::endl;
        }
    }

    delete[] unlocked;
    std::cout << "==========================" << std::endl;
}

// Skill point management
void AbilityTree::addSkillPoints(int points) {
    skillPoints += points;
    std::cout << "Gained " << points << " skill points! Total: " << skillPoints << std::endl;
}

int AbilityTree::getSkillPoints() const { return skillPoints; }

// Getters
int AbilityTree::getTotalAbilities() const { return totalAbilities; }
int AbilityTree::getUnlockedCount() const { return unlockedCount; }
AbilityNode* AbilityTree::getRoot() const { return root; }

// Helper: Collect unlocked abilities
void AbilityTree::collectUnlocked(AbilityNode* node, Ability** arr, int& index) const {
    if (node == nullptr) return;

    if (node->ability.unlocked) {
        arr[index++] = &(node->ability);
    }

    collectUnlocked(node->left, arr, index);
    collectUnlocked(node->right, arr, index);
}

// Helper: Collect available abilities
void AbilityTree::collectAvailable(AbilityNode* node, Ability** arr, int& index) const {
    if (node == nullptr) return;

    if (canUnlock(node->ability.code)) {
        arr[index++] = &(node->ability);
    }

    collectAvailable(node->left, arr, index);
    collectAvailable(node->right, arr, index);
}

// Get arrays of abilities
Ability** AbilityTree::getUnlockedAbilities(int& outCount) const {
    Ability** arr = new Ability*[totalAbilities];
    outCount = 0;
    collectUnlocked(root, arr, outCount);
    return arr;
}

Ability** AbilityTree::getAvailableAbilities(int& outCount) const {
    Ability** arr = new Ability*[totalAbilities];
    outCount = 0;
    collectAvailable(root, arr, outCount);
    return arr;
}

// Check if ability is unlocked
bool AbilityTree::isUnlocked(const std::string& code) const {
    AbilityNode* node = findNode(root, code);
    return (node != nullptr && node->ability.unlocked);
}

// Initialize default ability tree
void AbilityTree::initializeDefaultTree() {
    // Root ability (always unlocked)
    Ability basicCombat("BASIC_COMBAT", "Basic Combat",
        "Fundamental combat techniques", Ability::Type::COMBAT, 0, 10);
    setRoot(basicCombat);

    // Level 1 abilities (children of root)
    Ability timeDilation("TIME_DILATION", "Time Dilation",
        "Slow time to gain an extra action", Ability::Type::COMBAT, 2, 15);
    insert(timeDilation, "BASIC_COMBAT", true);

    Ability codeShield("CODE_SHIELD", "Code Shield",
        "Create a protective barrier", Ability::Type::DEFENSE, 2, 20);
    insert(codeShield, "BASIC_COMBAT", false);

    // Level 2 abilities
    Ability overdrive("OVERDRIVE", "Overdrive",
        "Massive damage boost for one attack", Ability::Type::COMBAT, 3, 30);
    insert(overdrive, "TIME_DILATION", true);

    Ability stealth("STEALTH_MODE", "Stealth Mode",
        "Become invisible to enemies", Ability::Type::SUPPORT, 3, 25);
    insert(stealth, "TIME_DILATION", false);

    Ability firewall("FIREWALL", "Personal Firewall",
        "Reflect damage back to attackers", Ability::Type::DEFENSE, 3, 20);
    insert(firewall, "CODE_SHIELD", true);

    Ability regen("REGENERATION", "System Regeneration",
        "Slowly recover health over time", Ability::Type::DEFENSE, 4, 15);
    insert(regen, "CODE_SHIELD", false);

    std::cout << "[AbilityTree] Default tree initialized." << std::endl;
}