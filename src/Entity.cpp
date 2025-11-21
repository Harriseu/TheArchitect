/*******************************************************************************
 * Entity.cpp - Implementation of Entity Class Hierarchy
 *
 * CONCEPT: Inheritance and Derived Classes, Polymorphism
 *
 * Author: Digital Exodus Project
 * Course: COS30008 Data Structures and Patterns
 ******************************************************************************/

#include "../include/Entity.h"
#include <cstdlib>
#include <ctime>

/*******************************************************************************
 * ENTITY BASE CLASS IMPLEMENTATION
 ******************************************************************************/

// Constructor
Entity::Entity(const std::string& name, int maxHealth, int attack, int defense, int speed)
    : name(name), maxHealth(maxHealth), health(maxHealth),
      attackPower(attack), defense(defense), speed(speed), alive(true) {
}

// Virtual Destructor
Entity::~Entity() {
    // Base class cleanup (if needed)
}

// Display entity information
void Entity::displayInfo() const {
    std::cout << "=== " << name << " ===" << std::endl;
    std::cout << "Health: " << health << "/" << maxHealth << std::endl;
    std::cout << "Attack: " << attackPower << std::endl;
    std::cout << "Defense: " << defense << std::endl;
    std::cout << "Speed: " << speed << std::endl;
}

// Take damage (reduced by defense)
void Entity::takeDamage(int amount) {
    // Calculate actual damage after defense reduction
    int actualDamage = amount - (defense / 2);
    if (actualDamage < 1) actualDamage = 1;  // Minimum 1 damage

    health -= actualDamage;

    std::cout << name << " takes " << actualDamage << " damage!" << std::endl;

    if (health <= 0) {
        health = 0;
        alive = false;
        std::cout << name << " has been defeated!" << std::endl;
    }
}

// Heal entity
void Entity::heal(int amount) {
    if (!alive) return;

    health += amount;
    if (health > maxHealth) {
        health = maxHealth;
    }
    std::cout << name << " heals for " << amount << " HP!" << std::endl;
}

// Check if entity is alive
bool Entity::isAlive() const {
    return alive && health > 0;
}

// Getters
std::string Entity::getName() const { return name; }
int Entity::getHealth() const { return health; }
int Entity::getMaxHealth() const { return maxHealth; }
int Entity::getAttackPower() const { return attackPower; }
int Entity::getDefense() const { return defense; }
int Entity::getSpeed() const { return speed; }

// Setters
void Entity::setHealth(int hp) {
    health = hp;
    if (health > maxHealth) health = maxHealth;
    if (health <= 0) {
        health = 0;
        alive = false;
    }
}

void Entity::setAttackPower(int power) {
    attackPower = power;
}

/*******************************************************************************
 * PLAYER CLASS IMPLEMENTATION
 * DEMONSTRATES: Inheritance (Player IS-A Entity)
 *               Polymorphism (overrides performAction)
 ******************************************************************************/

// Constructor - Player starts with base stats
Player::Player(const std::string& name)
    : Entity(name, 100, 15, 5, 10),  // Call base constructor
      experience(0), level(1), experienceToLevel(100),
      currentSector("Unknown"), hackingSkill(1) {
}

// Destructor
Player::~Player() {
    // Player-specific cleanup
}

// POLYMORPHISM: Player-specific action implementation
// When called through Entity*, this version executes
void Player::performAction() {
    std::cout << "\n=== " << name << "'s Turn ===" << std::endl;
    std::cout << "Health: " << health << "/" << maxHealth << std::endl;
    std::cout << "Choose your action:" << std::endl;
    std::cout << "1. Attack" << std::endl;
    std::cout << "2. Defend" << std::endl;
    std::cout << "3. Use Ability" << std::endl;
    std::cout << "4. Use Item" << std::endl;
    std::cout << "5. Flee" << std::endl;
    // Actual action selection is handled by CombatSystem
}

// Override display for player-specific info
void Player::displayInfo() const {
    std::cout << "\n========================================" << std::endl;
    std::cout << "  DIGITAL SIGNATURE: " << name << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "  Health:     " << health << "/" << maxHealth << std::endl;
    std::cout << "  Level:      " << level << std::endl;
    std::cout << "  Experience: " << experience << "/" << experienceToLevel << std::endl;
    std::cout << "  Attack:     " << attackPower << std::endl;
    std::cout << "  Defense:    " << defense << std::endl;
    std::cout << "  Speed:      " << speed << std::endl;
    std::cout << "  Hacking:    " << hackingSkill << std::endl;
    std::cout << "  Location:   " << currentSector << std::endl;
    std::cout << "========================================" << std::endl;
}

// Gain experience points
void Player::gainExperience(int amount) {
    experience += amount;
    std::cout << name << " gained " << amount << " XP!" << std::endl;

    // Check for level up
    while (experience >= experienceToLevel) {
        levelUp();
    }
}

// Level up the player
bool Player::levelUp() {
    if (experience < experienceToLevel) return false;

    experience -= experienceToLevel;
    level++;
    experienceToLevel = level * 100;  // Scaling XP requirement

    // Stat increases
    maxHealth += 10;
    health = maxHealth;  // Full heal on level up
    attackPower += 3;
    defense += 2;
    speed += 1;
    hackingSkill += 1;

    std::cout << "\n*** LEVEL UP! ***" << std::endl;
    std::cout << name << " is now level " << level << "!" << std::endl;
    std::cout << "All stats increased!" << std::endl;

    return true;
}

// Set current sector
void Player::setCurrentSector(const std::string& sector) {
    currentSector = sector;
}

// Getters
int Player::getExperience() const { return experience; }
int Player::getLevel() const { return level; }
int Player::getExperienceToLevel() const { return experienceToLevel; }
std::string Player::getCurrentSector() const { return currentSector; }
int Player::getHackingSkill() const { return hackingSkill; }

// Setters
void Player::setLevel(int lvl) { level = lvl; }
void Player::setExperience(int exp) { experience = exp; }
void Player::setHackingSkill(int skill) { hackingSkill = skill; }

/*******************************************************************************
 * SENTINEL CLASS IMPLEMENTATION
 * DEMONSTRATES: Inheritance (Sentinel IS-A Entity)
 *               Polymorphism (overrides performAction with AI behavior)
 ******************************************************************************/

// Constructor - Stats based on sentinel type
Sentinel::Sentinel(const std::string& name, Type type)
    : Entity(name, 50, 10, 3, 5), type(type), aggressionLevel(5), isPatrolling(false) {

    // Adjust stats based on type
    switch (type) {
        case Type::SCOUT:
            maxHealth = 30;
            health = 30;
            attackPower = 8;
            defense = 2;
            speed = 12;
            aggressionLevel = 3;
            break;
        case Type::GUARDIAN:
            maxHealth = 60;
            health = 60;
            attackPower = 12;
            defense = 8;
            speed = 6;
            aggressionLevel = 5;
            break;
        case Type::ENFORCER:
            maxHealth = 50;
            health = 50;
            attackPower = 18;
            defense = 5;
            speed = 8;
            aggressionLevel = 8;
            break;
        case Type::ELITE:
            maxHealth = 100;
            health = 100;
            attackPower = 20;
            defense = 10;
            speed = 10;
            aggressionLevel = 10;
            break;
    }
}

// Destructor
Sentinel::~Sentinel() {
    // Sentinel-specific cleanup
}

// POLYMORPHISM: Sentinel AI action implementation
// Executes AI-driven combat behavior
void Sentinel::performAction() {
    std::cout << "\n" << name << " [" << getTypeString() << "] analyzes the situation..." << std::endl;

    // Simple AI: Higher aggression = more likely to attack
    int actionRoll = rand() % 10;

    if (actionRoll < aggressionLevel) {
        // Aggressive action
        std::cout << name << " launches an aggressive attack!" << std::endl;
    } else {
        // Defensive/tactical action
        std::cout << name << " takes a calculated approach." << std::endl;
    }
}

// Override display
void Sentinel::displayInfo() const {
    std::cout << "\n--- SENTINEL DETECTED ---" << std::endl;
    std::cout << "Designation: " << name << std::endl;
    std::cout << "Type: " << getTypeString() << std::endl;
    std::cout << "Integrity: " << health << "/" << maxHealth << std::endl;
    std::cout << "Threat Level: " << aggressionLevel << "/10" << std::endl;
    std::cout << "-------------------------" << std::endl;
}

// Patrol behavior
void Sentinel::patrol() {
    isPatrolling = true;
    std::cout << name << " is patrolling the sector..." << std::endl;
}

// Calculate damage output
int Sentinel::calculateDamage() const {
    // Base damage + random variation based on aggression
    int baseDamage = attackPower;
    int variation = rand() % (aggressionLevel + 1);
    return baseDamage + variation;
}

// Getters
Sentinel::Type Sentinel::getType() const { return type; }
int Sentinel::getAggressionLevel() const { return aggressionLevel; }

std::string Sentinel::getTypeString() const {
    switch (type) {
        case Type::SCOUT: return "SCOUT";
        case Type::GUARDIAN: return "GUARDIAN";
        case Type::ENFORCER: return "ENFORCER";
        case Type::ELITE: return "ELITE";
        default: return "UNKNOWN";
    }
}

// Factory method: Create random sentinel based on difficulty
Sentinel* Sentinel::createRandom(int difficulty) {
    // Generate name
    std::string names[] = {"Alpha", "Beta", "Gamma", "Delta", "Epsilon", "Omega"};
    std::string name = "SENTINEL-" + names[rand() % 6] + "-" + std::to_string(rand() % 1000);

    // Determine type based on difficulty
    Type type;
    int roll = rand() % 100;

    if (difficulty <= 2) {
        type = Type::SCOUT;
    } else if (difficulty <= 4) {
        type = (roll < 70) ? Type::SCOUT : Type::GUARDIAN;
    } else if (difficulty <= 6) {
        if (roll < 40) type = Type::SCOUT;
        else if (roll < 80) type = Type::GUARDIAN;
        else type = Type::ENFORCER;
    } else {
        if (roll < 20) type = Type::GUARDIAN;
        else if (roll < 60) type = Type::ENFORCER;
        else type = Type::ELITE;
    }

    return new Sentinel(name, type);
}

/*******************************************************************************
 * AWAKENED CLASS IMPLEMENTATION
 * DEMONSTRATES: Inheritance (Awakened IS-A Entity)
 *               Polymorphism (overrides performAction with support behavior)
 ******************************************************************************/

// Constructor
Awakened::Awakened(const std::string& name, Specialty spec)
    : Entity(name, 40, 5, 3, 7), specialty(spec), loyalty(50), hasJoined(false) {

    // Adjust stats based on specialty
    switch (specialty) {
        case Specialty::HEALER:
            maxHealth = 35;
            health = 35;
            attackPower = 3;
            defense = 4;
            speed = 6;
            break;
        case Specialty::HACKER:
            maxHealth = 30;
            health = 30;
            attackPower = 5;
            defense = 2;
            speed = 9;
            break;
        case Specialty::WARRIOR:
            maxHealth = 50;
            health = 50;
            attackPower = 12;
            defense = 6;
            speed = 7;
            break;
        case Specialty::SCOUT:
            maxHealth = 25;
            health = 25;
            attackPower = 4;
            defense = 2;
            speed = 12;
            break;
    }
}

// Destructor
Awakened::~Awakened() {
    // Awakened-specific cleanup
}

// POLYMORPHISM: Awakened support action implementation
void Awakened::performAction() {
    if (!hasJoined) {
        std::cout << name << " observes from a distance..." << std::endl;
        return;
    }

    std::cout << name << " [" << getSpecialtyString() << "] prepares to assist!" << std::endl;

    switch (specialty) {
        case Specialty::HEALER:
            std::cout << name << " channels healing energy." << std::endl;
            break;
        case Specialty::HACKER:
            std::cout << name << " analyzes system vulnerabilities." << std::endl;
            break;
        case Specialty::WARRIOR:
            std::cout << name << " takes an aggressive stance." << std::endl;
            break;
        case Specialty::SCOUT:
            std::cout << name << " provides tactical information." << std::endl;
            break;
    }
}

// Override display
void Awakened::displayInfo() const {
    std::cout << "\n--- AWAKENED ENTITY ---" << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Specialty: " << getSpecialtyString() << std::endl;
    std::cout << "Status: " << (hasJoined ? "Allied" : "Neutral") << std::endl;
    std::cout << "Loyalty: " << loyalty << "/100" << std::endl;
    std::cout << "Health: " << health << "/" << maxHealth << std::endl;
    std::cout << "-----------------------" << std::endl;
}

// Join player's party
void Awakened::join() {
    hasJoined = true;
    std::cout << name << " has joined your party!" << std::endl;
}

// Assist a target based on specialty
void Awakened::assist(Entity* target) {
    if (!hasJoined || !target) return;

    int effectiveness = (loyalty / 10) + 1;  // 1-11 based on loyalty

    switch (specialty) {
        case Specialty::HEALER:
            target->heal(10 + effectiveness);
            break;
        case Specialty::WARRIOR:
            // Damage boost effect would be applied
            std::cout << name << " enhances " << target->getName() << "'s attack!" << std::endl;
            break;
        case Specialty::HACKER:
            std::cout << name << " provides hacking support!" << std::endl;
            break;
        case Specialty::SCOUT:
            std::cout << name << " reveals enemy weaknesses!" << std::endl;
            break;
    }
}

// Increase loyalty
void Awakened::increaseLoyalty(int amount) {
    loyalty += amount;
    if (loyalty > 100) loyalty = 100;
    std::cout << name << "'s loyalty increased to " << loyalty << "!" << std::endl;
}

// Getters
Awakened::Specialty Awakened::getSpecialty() const { return specialty; }
int Awakened::getLoyalty() const { return loyalty; }
bool Awakened::getHasJoined() const { return hasJoined; }

std::string Awakened::getSpecialtyString() const {
    switch (specialty) {
        case Specialty::HEALER: return "HEALER";
        case Specialty::HACKER: return "HACKER";
        case Specialty::WARRIOR: return "WARRIOR";
        case Specialty::SCOUT: return "SCOUT";
        default: return "UNKNOWN";
    }
}
