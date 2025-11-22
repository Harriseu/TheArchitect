/*******************************************************************************
 * Tree.h - Tree Implementation for Ability/Skill System
 *
 * CONCEPT: Tree (Abstract Data Type)
 *
 * This tree represents the player's skill progression system.
 * Root contains basic abilities, children are advanced skills.
 * Players must unlock parent nodes before accessing children.
 *
 * WHY TREE:
 * - Hierarchical nature matches skill prerequisites
 * - Parent-child relationship models dependency
 * - Flat list can't represent "requires X to unlock Y"
 * - Graph would be overkill for clear hierarchies
 *
 * Author: Digital Exodus Project
 * Course: COS30008 Data Structures and Patterns
 ******************************************************************************/

#ifndef TREE_H
#define TREE_H

#include <string>
#include <iostream>

/*******************************************************************************
 * Ability Structure - Represents a skill/ability
 ******************************************************************************/
struct Ability {
    std::string code;           // Unique identifier
    std::string name;           // Display name
    std::string description;    // What it does

    enum class Type {
        COMBAT,                 // Damage abilities
        DEFENSE,                // Protective abilities
        HACKING,                // System manipulation
        SUPPORT                 // Utility abilities
    } type;

    int cost;                   // Skill points to unlock
    int power;                  // Effect strength
    bool unlocked;              // Has player unlocked this?

    Ability();
    Ability(const std::string& code, const std::string& name,
            const std::string& desc, Type type, int cost, int power);
};

/*******************************************************************************
 * CONCEPT: TREE - Ability Node
 *
 * Each node contains an ability and pointers to child abilities.
 * Using binary tree structure (left = first child, right = sibling)
 * This allows variable number of children while using binary pointers.
 ******************************************************************************/
struct AbilityNode {
    Ability ability;            // The ability at this node
    AbilityNode* left;          // Left child (first prerequisite path)
    AbilityNode* right;         // Right child (second prerequisite path)
    AbilityNode* parent;        // Parent node (for prerequisite checking)

    AbilityNode(const Ability& ability);
    ~AbilityNode();
};

/*******************************************************************************
 * CONCEPT: TREE IMPLEMENTATION - AbilityTree
 *
 * Operations:
 * - insert(ability, parentCode): Add ability as child of parent
 * - unlock(code): Unlock ability if prerequisites met
 * - canUnlock(code): Check if prerequisites are satisfied
 * - findAbility(code): Locate ability by code
 * - displayTree(): Show full skill tree
 * - displayAvailable(): Show abilities that can be unlocked
 *
 * Structure Example:
 *                    [Basic Combat]
 *                    /            \
 *           [Time Dilation]    [Code Shield]
 *              /      \           /        \
 *        [Overdrive] [Stealth] [Firewall] [Regen]
 *
 * Traversal: Pre-order for displaying tree structure
 ******************************************************************************/
class AbilityTree {
private:
    AbilityNode* root;          // Root of the tree
    int totalAbilities;         // Count of all abilities
    int unlockedCount;          // Count of unlocked abilities
    int skillPoints;            // Available skill points

    // Helper functions
    AbilityNode* findNode(AbilityNode* node, const std::string& code) const;
    void displayNode(AbilityNode* node, int depth) const;
    void deleteTree(AbilityNode* node);
    void collectUnlocked(AbilityNode* node, Ability** arr, int& index) const;
    void collectAvailable(AbilityNode* node, Ability** arr, int& index) const;

public:
    // Constructor - initializes empty tree
    AbilityTree();

    // Destructor - frees all nodes
    ~AbilityTree();

    // Set root ability
    void setRoot(const Ability& ability);

    // Insert: Add ability as child of parent
    // Step 1: Find parent node by code
    // Step 2: Create new AbilityNode
    // Step 3: Attach as left or right child
    // Step 4: Set parent pointer
    // Step 5: Increment total count
    bool insert(const Ability& ability, const std::string& parentCode, bool asLeftChild);

    // Find: Locate ability by code
    // Step 1: Start at root
    // Step 2: Recursively search left and right subtrees
    // Step 3: Return node if found, nullptr otherwise
    Ability* findAbility(const std::string& code) const;

    // CanUnlock: Check if ability can be unlocked
    // Step 1: Find the ability node
    // Step 2: Check if parent is unlocked (or is root)
    // Step 3: Check if player has enough skill points
    // Step 4: Check if not already unlocked
    bool canUnlock(const std::string& code) const;

    // Unlock: Attempt to unlock an ability
    // Step 1: Verify canUnlock returns true
    // Step 2: Deduct skill points
    // Step 3: Mark ability as unlocked
    // Step 4: Increment unlocked count
    bool unlock(const std::string& code);

    // Display functions
    void displayTree() const;           // Show full tree structure
    void displayAvailable() const;      // Show unlockable abilities
    void displayUnlocked() const;       // Show player's unlocked abilities

    // Skill point management
    void addSkillPoints(int points);
    int getSkillPoints() const;

    // Getters
    int getTotalAbilities() const;
    int getUnlockedCount() const;
    AbilityNode* getRoot() const;

    // Get arrays of abilities
    Ability** getUnlockedAbilities(int& outCount) const;
    Ability** getAvailableAbilities(int& outCount) const;

    // Initialize default ability tree
    void initializeDefaultTree();

    // Check if specific ability is unlocked
    bool isUnlocked(const std::string& code) const;
};

#endif // TREE_H
