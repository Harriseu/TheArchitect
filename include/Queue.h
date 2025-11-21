/*******************************************************************************
 * Queue.h - Queue (FIFO) Implementation for Combat Turn System
 *
 * CONCEPT: Queue (Abstract Data Type)
 *
 * This queue manages the turn-based combat system.
 * Combatants are enqueued based on speed/initiative.
 * Each entity is dequeued for their turn, then re-enqueued.
 *
 * WHY QUEUE:
 * - FIFO (First In, First Out) ensures fair turn order
 * - First entity to be queued acts first
 * - Stack would reverse order (last in acts first - unfair)
 * - Natural model for sequential turn-taking
 *
 * Author: Alif Harriz Jeffery | 102782711
 * Course: COS30008 Data Structures and Patterns
 ******************************************************************************/

#ifndef QUEUE_H
#define QUEUE_H

#include <string>
#include <iostream>

// Forward declaration
class Entity;

/*******************************************************************************
 * Queue Node - Node for linked-list based queue
 ******************************************************************************/
template<typename T>
struct QueueNode {
    T data;                     // Stored data
    QueueNode<T>* next;         // Pointer to next node in queue

    QueueNode(const T& data) : data(data), next(nullptr) {}
};

/*******************************************************************************
 * CONCEPT: QUEUE (FIFO) IMPLEMENTATION
 *
 * Operations:
 * - enqueue(data): Add element to rear - O(1)
 * - dequeue(): Remove and return front element - O(1)
 * - peek(): View front element without removing - O(1)
 * - isEmpty(): Check if queue is empty - O(1)
 * - getSize(): Return number of elements - O(1)
 *
 * Structure:
 * FRONT -> [Entity1] -> [Entity2] -> [Entity3] <- REAR
 * (first out)                         (last in)
 *
 * FIFO Behavior:
 * - enqueue(A), enqueue(B), enqueue(C)
 * - dequeue() returns A, dequeue() returns B, dequeue() returns C
 ******************************************************************************/
template<typename T>
class Queue {
private:
    QueueNode<T>* front;        // Points to front of queue (dequeue here)
    QueueNode<T>* rear;         // Points to rear of queue (enqueue here)
    int size;                   // Number of elements

public:
    // Constructor - initializes empty queue
    Queue() : front(nullptr), rear(nullptr), size(0) {}

    // Destructor - frees all nodes
    ~Queue() {
        clear();
    }

    // Enqueue: Add element to rear of queue
    // Step 1: Create new QueueNode with data
    // Step 2: If queue empty, set both front and rear to new node
    // Step 3: Otherwise, link current rear to new node
    // Step 4: Update rear to point to new node
    // Step 5: Increment size
    void enqueue(const T& data) {
        // Create new node
        QueueNode<T>* newNode = new QueueNode<T>(data);

        // If queue is empty
        if (isEmpty()) {
            front = newNode;
            rear = newNode;
        } else {
            // Link current rear to new node
            rear->next = newNode;
            // Update rear pointer
            rear = newNode;
        }

        // Increment size
        size++;
    }

    // Dequeue: Remove and return front element
    // Step 1: Check if queue is empty (return default if so)
    // Step 2: Store front's data
    // Step 3: Save reference to front node
    // Step 4: Move front pointer to next node
    // Step 5: If queue becomes empty, update rear to null
    // Step 6: Delete old front node
    // Step 7: Decrement size and return data
    T dequeue() {
        // Check for empty queue
        if (isEmpty()) {
            return T(); // Return default-constructed T
        }

        // Store data to return
        T data = front->data;

        // Save reference to node being removed
        QueueNode<T>* temp = front;

        // Move front to next node
        front = front->next;

        // If queue is now empty, update rear
        if (front == nullptr) {
            rear = nullptr;
        }

        // Delete old front
        delete temp;

        // Decrement size
        size--;

        return data;
    }

    // Peek: View front element without removing
    // Step 1: Check if queue is empty
    // Step 2: Return front's data (no modification)
    T peek() const {
        if (isEmpty()) {
            return T(); // Return default-constructed T
        }
        return front->data;
    }

    // isEmpty: Check if queue has no elements
    bool isEmpty() const {
        return front == nullptr;
    }

    // getSize: Return number of elements
    int getSize() const {
        return size;
    }

    // Clear: Remove all elements
    // Step 1: While queue not empty
    // Step 2: Dequeue each element
    void clear() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    // Contains: Check if element exists in queue
    bool contains(const T& data) const {
        QueueNode<T>* current = front;
        while (current != nullptr) {
            if (current->data == data) {
                return true;
            }
            current = current->next;
        }
        return false;
    }
};

/*******************************************************************************
 * CombatQueue - Specialized queue for combat turn management
 *
 * Manages turn order for all combatants (player, enemies, allies).
 * Entities are sorted by speed when initializing combat.
 ******************************************************************************/
class CombatQueue {
private:
    Queue<Entity*>* turnQueue;  // Queue of entity pointers
    int roundNumber;            // Current combat round

public:
    CombatQueue();
    ~CombatQueue();

    // Combat initialization
    // Step 1: Collect all combatants
    // Step 2: Sort by speed (highest first)
    // Step 3: Enqueue in sorted order
    void initializeCombat(Entity** combatants, int count);

    // Turn management
    Entity* getNextTurn();      // Dequeue and return next entity
    void addToCombat(Entity* entity);  // Add entity to turn order
    void removeFromCombat(Entity* entity);  // Remove defeated entity

    // Round management
    void nextRound();           // Increment round counter
    int getRoundNumber() const;

    // Status
    bool isCombatOver() const;
    int getCombatantCount() const;
    Entity* peekNextTurn() const;

    // Display
    void displayTurnOrder() const;
    void displayCombatStatus() const;

    // Clear combat
    void endCombat();
};

#endif // QUEUE_H