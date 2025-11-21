/*******************************************************************************
 * Entity.h - Base Entity Class and Derived Classes
 *
 * CONCEPT: Inheritance and Derived Classes, Polymorphism
 *
 * This file demonstrates Object-Oriented Programming concepts:
 * - Entity is the abstract base class for all game entities
 * - Player, Sentinel, and Awakened are derived classes
 * - Virtual functions enable polymorphic behavior
 *
 * Author: Alif Harriz Jeffery | 102782711
 * Course: COS30008 Data Structures and Patterns
 ******************************************************************************/

#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <iostream>

// Forward declarations
class Inventory;
class AbilityTree;

/*******************************************************************************
 * CONCEPT 1: INHERITANCE - Base Entity Class
 *
 * The Entity class serves as the root of our inheritance hierarchy.
 * All game characters (Player, Sentinel, Awakened) inherit from this class.
 * This demonstrates the IS-A relationship in OOP.
 ******************************************************************************/
class Entity {
protected:
    std::string name;           // Entity's identifier
    int health;                 // Current health points
    int maxHealth;              // Maximum health points
    int attackPower;            // Base attack damage
    int defense;                // Damage reduction
    int speed;                  // Turn order priority
    bool alive;                 // Living status

public:
    // Constructor and Virtual Destructor
    Entity(const std::string& name, int maxHealth, int attack, int defense, int speed);
    virtual ~Entity();

    // CONCEPT 2: POLYMORPHISM - Pure Virtual Function
    // Each derived class MUST implement this differently
    // This enables runtime polymorphism when iterating over Entity pointers
    virtual void performAction() = 0;

    // Virtual function for displaying entity information
    // Can be overridden by derived classes for specialized display
    virtual void displayInfo() const;

    // Common operations for all entities
    void takeDamage(int amount);
    void heal(int amount);
    bool isAlive() const;

    // Getters
    std::string getName() const;
    int getHealth() const;
    int getMaxHealth() const;
    int getAttackPower() const;
    int getDefense() const;
    int getSpeed() const;

    // Setters
    void setHealth(int hp);
    void setAttackPower(int power);
};

/*******************************************************************************
 * CONCEPT 1 & 2: Player Class - Derived from Entity
 *
 * Demonstrates INHERITANCE: Player IS-A Entity
 * Demonstrates POLYMORPHISM: Overrides performAction() with player-specific behavior
 ******************************************************************************/
class Player : public Entity {
private:
    int experience;             // Experience points
    int level;                  // Current level
    int experienceToLevel;      // XP needed for next level
    std::string currentSector;  // Current location name
    int hackingSkill;           // Proficiency in hacking

public:
    // Constructor
    Player(const std::string& name);

    // Destructor
    ~Player() override;

    // POLYMORPHISM: Player-specific action implementation
    // When called through Entity*, executes player's turn logic
    void performAction() override;

    // Override display for player-specific info
    void displayInfo() const override;

    // Player-specific methods
    void gainExperience(int amount);
    bool levelUp();
    void setCurrentSector(const std::string& sector);

    // Getters
    int getExperience() const;
    int getLevel() const;
    int getExperienceToLevel() const;
    std::string getCurrentSector() const;
    int getHackingSkill() const;

    // Setters
    void setLevel(int lvl);
    void setExperience(int exp);
    void setHackingSkill(int skill);
};

/*******************************************************************************
 * CONCEPT 1 & 2: Sentinel Class - Derived from Entity
 *
 * Sentinels are enemy AI entities that hunt the player.
 * Demonstrates INHERITANCE and POLYMORPHISM with AI-driven behavior.
 ******************************************************************************/
class Sentinel : public Entity {
public:
    // Sentinel types with different behaviors
    enum class Type {
        SCOUT,      // Low health, fast, weak
        GUARDIAN,   // Medium stats, balanced
        ENFORCER,   // High attack, medium defense
        ELITE       // High stats across the board
    };

private:
    Type type;                  // Sentinel classification
    int aggressionLevel;        // AI aggression (1-10)
    bool isPatrolling;          // Current state

public:
    // Constructor
    Sentinel(const std::string& name, Type type);

    // Destructor
    ~Sentinel() override;

    // POLYMORPHISM: Sentinel AI action implementation
    // Executes AI-driven combat behavior
    void performAction() override;

    // Override display for sentinel-specific info
    void displayInfo() const override;

    // AI behavior methods
    void patrol();
    int calculateDamage() const;

    // Getters
    Type getType() const;
    int getAggressionLevel() const;
    std::string getTypeString() const;

    // Static factory method to create random sentinels
    static Sentinel* createRandom(int difficulty);
};

/*******************************************************************************
 * CONCEPT 1 & 2: Awakened Class - Derived from Entity
 *
 * Awakened are NPC allies that can join and assist the player.
 * Demonstrates INHERITANCE and POLYMORPHISM with support behavior.
 ******************************************************************************/
class Awakened : public Entity {
public:
    // Awakened specialties
    enum class Specialty {
        HEALER,     // Restores health
        HACKER,     // Assists with hacking
        WARRIOR,    // Combat support
        SCOUT       // Reveals information
    };

private:
    Specialty specialty;        // NPC's role
    int loyalty;                // Loyalty level (affects effectiveness)
    bool hasJoined;             // Whether they've joined the party

public:
    // Constructor
    Awakened(const std::string& name, Specialty spec);

    // Destructor
    ~Awakened() override;

    // POLYMORPHISM: Awakened support action implementation
    void performAction() override;

    // Override display for awakened-specific info
    void displayInfo() const override;

    // NPC-specific methods
    void join();
    void assist(Entity* target);
    void increaseLoyalty(int amount);

    // Getters
    Specialty getSpecialty() const;
    int getLoyalty() const;
    bool getHasJoined() const;
    std::string getSpecialtyString() const;
};

#endif // ENTITY_H