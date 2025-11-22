/*******************************************************************************
 * Stack.h - Stack (LIFO) Implementation for Undo System
 *
 * CONCEPT: Stack (Abstract Data Type)
 *
 * This stack implements the undo/rollback system for the hacking minigame.
 * When player inputs code sequences during hacking, actions are pushed.
 * Player can undo actions with limited "Revert" tokens.
 *
 * WHY STACK:
 * - LIFO (Last In, First Out) nature matches undo semantics
 * - Most recent action should be undone first
 * - Queue would undo oldest first (wrong order)
 * - Simple operations: push, pop, peek
 *
 * Author: Alif Harriz Jeffery | 102782711
 * Course: COS30008 Data Structures and Patterns
 ******************************************************************************/

#ifndef STACK_H
#define STACK_H

#include <string>
#include <iostream>

/*******************************************************************************
 * Action Structure - Represents an undoable action
 ******************************************************************************/
struct Action {
    std::string actionType;     // Type of action (INJECT, BYPASS, DECRYPT)
    std::string description;    // Human-readable description
    int value;                  // Associated value (damage, cost, etc.)
    bool successful;            // Whether action succeeded

    Action();
    Action(const std::string& type, const std::string& desc, int val, bool success);
};

/*******************************************************************************
 * Stack Node - Node for linked-list based stack
 ******************************************************************************/
template<typename T>
struct StackNode {
    T data;                     // Stored data
    StackNode<T>* next;         // Pointer to node below in stack

    StackNode(const T& data) : data(data), next(nullptr) {}
};

/*******************************************************************************
 * CONCEPT: STACK (LIFO) IMPLEMENTATION
 *
 * Operations:
 * - push(data): Add element to top - O(1)
 * - pop(): Remove and return top element - O(1)
 * - peek(): View top element without removing - O(1)
 * - isEmpty(): Check if stack is empty - O(1)
 * - getSize(): Return number of elements - O(1)
 *
 * Structure:
 * TOP -> [Action3] -> [Action2] -> [Action1] -> nullptr
 * (newest)                            (oldest)
 *
 * LIFO Behavior:
 * - push(A), push(B), push(C)
 * - pop() returns C, pop() returns B, pop() returns A
 ******************************************************************************/
template<typename T>
class Stack {
private:
    StackNode<T>* top;          // Pointer to top of stack
    int size;                   // Number of elements

public:
    // Constructor - initializes empty stack
    Stack() : top(nullptr), size(0) {}

    // Destructor - frees all nodes
    ~Stack() {
        clear();
    }

    // Push: Add element to top of stack
    // Step 1: Create new StackNode with data
    // Step 2: Set new node's next pointer to current top
    // Step 3: Update top to point to new node
    // Step 4: Increment size
    void push(const T& data) {
        // Create new node
        StackNode<T>* newNode = new StackNode<T>(data);

        // Link new node to current top
        newNode->next = top;

        // Update top pointer
        top = newNode;

        // Increment size counter
        size++;
    }

    // Pop: Remove and return top element
    // Step 1: Check if stack is empty (return default if so)
    // Step 2: Store top's data
    // Step 3: Save reference to top node
    // Step 4: Move top pointer to next node
    // Step 5: Delete old top node
    // Step 6: Decrement size and return data
    T pop() {
        // Check for empty stack
        if (isEmpty()) {
            return T(); // Return default-constructed T
        }

        // Store data to return
        T data = top->data;

        // Save reference to node being removed
        StackNode<T>* temp = top;

        // Move top to next node
        top = top->next;

        // Delete old top
        delete temp;

        // Decrement size
        size--;

        return data;
    }

    // Peek: View top element without removing
    // Step 1: Check if stack is empty
    // Step 2: Return top's data (no modification)
    T peek() const {
        if (isEmpty()) {
            return T(); // Return default-constructed T
        }
        return top->data;
    }

    // isEmpty: Check if stack has no elements
    bool isEmpty() const {
        return top == nullptr;
    }

    // getSize: Return number of elements
    int getSize() const {
        return size;
    }

    // Clear: Remove all elements
    // Step 1: While stack not empty
    // Step 2: Pop each element
    void clear() {
        while (!isEmpty()) {
            pop();
        }
    }

    // Display: Show stack contents (for debugging)
    void display() const {
        if (isEmpty()) {
            std::cout << "[Stack is empty]\n";
            return;
        }

        std::cout << "Stack (top to bottom):\n";
        StackNode<T>* current = top;
        int index = 0;
        while (current != nullptr) {
            std::cout << "  [" << index++ << "] ";
            // Note: This requires T to have operator<< defined
            // For Action type, we handle this specifically
            current = current->next;
        }
    }
};

/*******************************************************************************
 * ActionStack - Specialized stack for Action objects
 *
 * Used in the hacking minigame for undo functionality.
 ******************************************************************************/
class ActionStack {
private:
    Stack<Action>* stack;
    int maxRevertTokens;        // Maximum undo attempts
    int revertTokens;           // Current undo attempts remaining

public:
    ActionStack(int maxTokens = 3);
    ~ActionStack();

    // Action management
    void pushAction(const Action& action);
    Action popAction();
    Action peekAction() const;

    // Revert system
    bool canRevert() const;
    bool revert();              // Use a revert token to undo
    void resetTokens();         // Reset revert tokens

    // Status
    bool isEmpty() const;
    int getSize() const;
    int getRemainingTokens() const;

    // Display
    void displayActions() const;
    void displayStatus() const;

    // Clear all actions
    void clear();
};

#endif // STACK_H