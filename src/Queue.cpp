/*******************************************************************************
 * Queue.cpp - Queue Implementation for Combat Turn System
 *
 * CONCEPT: Queue (Abstract Data Type)
 *
 * Author: Alif Harriz Jeffery | 102782711
 * Course: COS30008 Data Structures and Patterns
 ******************************************************************************/

#include "../include/Queue.h"
#include "../include/Entity.h"

/*******************************************************************************
 * COMBAT QUEUE IMPLEMENTATION
 *
 * Manages turn order in combat using FIFO queue.
 * Entities are sorted by speed when combat starts.
 ******************************************************************************/

CombatQueue::CombatQueue() : roundNumber(0) {
    turnQueue = new Queue<Entity*>();
}

CombatQueue::~CombatQueue() {
    delete turnQueue;
}

// INITIALIZE COMBAT: Set up turn order
// Step 1: Collect all combatants
// Step 2: Sort by speed (highest first) using simple bubble sort
// Step 3: Enqueue in sorted order
void CombatQueue::initializeCombat(Entity** combatants, int count) {
    // Clear any existing queue
    turnQueue->clear();
    roundNumber = 1;

    if (count <= 0 || combatants == nullptr) return;

    // Step 1 & 2: Create sorted copy (bubble sort by speed, descending)
    Entity** sorted = new Entity*[count];
    for (int i = 0; i < count; i++) {
        sorted[i] = combatants[i];
    }

    // Bubble sort by speed (descending - fastest first)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (sorted[j]->getSpeed() < sorted[j + 1]->getSpeed()) {
                Entity* temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
    }

    // Step 3: Enqueue in sorted order
    std::cout << "\n=== COMBAT INITIATED ===" << std::endl;
    std::cout << "Turn order (by speed):" << std::endl;

    for (int i = 0; i < count; i++) {
        if (sorted[i] != nullptr && sorted[i]->isAlive()) {
            turnQueue->enqueue(sorted[i]);
            std::cout << "  " << (i + 1) << ". " << sorted[i]->getName()
                      << " (Speed: " << sorted[i]->getSpeed() << ")" << std::endl;
        }
    }

    std::cout << "========================" << std::endl;

    delete[] sorted;
}

// GET NEXT TURN: Dequeue and return next entity
Entity* CombatQueue::getNextTurn() {
    if (turnQueue->isEmpty()) {
        return nullptr;
    }

    // FIFO: First entity in queue gets their turn
    Entity* next = turnQueue->dequeue();

    // Skip dead entities
    while (next != nullptr && !next->isAlive()) {
        if (turnQueue->isEmpty()) {
            return nullptr;
        }
        next = turnQueue->dequeue();
    }

    return next;
}

// Add entity to combat (during combat)
void CombatQueue::addToCombat(Entity* entity) {
    if (entity != nullptr && entity->isAlive()) {
        turnQueue->enqueue(entity);
        std::cout << entity->getName() << " enters combat!" << std::endl;
    }
}

// Remove defeated entity (they won't be re-enqueued)
void CombatQueue::removeFromCombat(Entity* entity) {
    // The entity is simply not re-enqueued after their turn
    // when they are defeated (isAlive() returns false)
    std::cout << entity->getName() << " removed from combat." << std::endl;
}

// Advance to next round
void CombatQueue::nextRound() {
    roundNumber++;
    std::cout << "\n*** ROUND " << roundNumber << " ***" << std::endl;
}

int CombatQueue::getRoundNumber() const {
    return roundNumber;
}

// Check if combat should end
bool CombatQueue::isCombatOver() const {
    return turnQueue->isEmpty();
}

int CombatQueue::getCombatantCount() const {
    return turnQueue->getSize();
}

Entity* CombatQueue::peekNextTurn() const {
    return turnQueue->peek();
}

// Display current turn order
void CombatQueue::displayTurnOrder() const {
    std::cout << "\n--- Turn Order ---" << std::endl;
    std::cout << "Round: " << roundNumber << std::endl;
    std::cout << "Combatants remaining: " << turnQueue->getSize() << std::endl;

    if (!turnQueue->isEmpty()) {
        Entity* next = turnQueue->peek();
        if (next != nullptr) {
            std::cout << "Next up: " << next->getName() << std::endl;
        }
    }
    std::cout << "------------------" << std::endl;
}

// Display combat status
void CombatQueue::displayCombatStatus() const {
    std::cout << "\n=== COMBAT STATUS ===" << std::endl;
    std::cout << "Round: " << roundNumber << std::endl;
    std::cout << "Active combatants: " << turnQueue->getSize() << std::endl;
    std::cout << "=====================" << std::endl;
}

// End combat and clean up
void CombatQueue::endCombat() {
    turnQueue->clear();
    roundNumber = 0;
    std::cout << "\n=== COMBAT ENDED ===" << std::endl;
}