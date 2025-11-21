/*******************************************************************************
 * Stack.cpp - Stack Implementation for Undo System
 *
 * CONCEPT: Stack (Abstract Data Type)
 *
 * Author: Digital Exodus Project
 * Course: COS30008 Data Structures and Patterns
 ******************************************************************************/

#include "../include/Stack.h"

/*******************************************************************************
 * ACTION STRUCTURE IMPLEMENTATION
 ******************************************************************************/

Action::Action() : actionType(""), description(""), value(0), successful(false) {
}

Action::Action(const std::string& type, const std::string& desc, int val, bool success)
    : actionType(type), description(desc), value(val), successful(success) {
}

/*******************************************************************************
 * ACTION STACK IMPLEMENTATION
 *
 * Specialized stack for the hacking minigame undo system.
 * Uses Stack<Action> internally with revert token management.
 ******************************************************************************/

ActionStack::ActionStack(int maxTokens)
    : maxRevertTokens(maxTokens), revertTokens(maxTokens) {
    stack = new Stack<Action>();
}

ActionStack::~ActionStack() {
    delete stack;
}

// Push action onto stack
void ActionStack::pushAction(const Action& action) {
    stack->push(action);
    std::cout << "[ActionStack] Action recorded: " << action.description << std::endl;
}

// Pop action from stack
Action ActionStack::popAction() {
    return stack->pop();
}

// Peek at top action
Action ActionStack::peekAction() const {
    return stack->peek();
}

// Check if revert is possible
bool ActionStack::canRevert() const {
    return !stack->isEmpty() && revertTokens > 0;
}

// REVERT: Use a revert token to undo last action
// Step 1: Check if revert is possible
// Step 2: Decrement revert tokens
// Step 3: Pop and process the action (undo its effect)
// Step 4: Return success
bool ActionStack::revert() {
    // Step 1: Check if we can revert
    if (!canRevert()) {
        if (revertTokens <= 0) {
            std::cout << "[ActionStack] No revert tokens remaining!" << std::endl;
        } else {
            std::cout << "[ActionStack] No actions to undo!" << std::endl;
        }
        return false;
    }

    // Step 2: Use a revert token
    revertTokens--;

    // Step 3: Pop the action
    Action undone = stack->pop();

    std::cout << "\n*** REVERT ACTIVATED ***" << std::endl;
    std::cout << "Undoing: " << undone.description << std::endl;
    std::cout << "Revert tokens remaining: " << revertTokens << "/" << maxRevertTokens << std::endl;

    // Step 4: Return success
    return true;
}

// Reset revert tokens (after successful hack or new session)
void ActionStack::resetTokens() {
    revertTokens = maxRevertTokens;
    std::cout << "[ActionStack] Revert tokens reset to " << maxRevertTokens << std::endl;
}

// Status checks
bool ActionStack::isEmpty() const {
    return stack->isEmpty();
}

int ActionStack::getSize() const {
    return stack->getSize();
}

int ActionStack::getRemainingTokens() const {
    return revertTokens;
}

// Display action history
void ActionStack::displayActions() const {
    std::cout << "\n=== ACTION HISTORY (Stack) ===" << std::endl;
    std::cout << "Actions in stack: " << stack->getSize() << std::endl;
    std::cout << "Revert tokens: " << revertTokens << "/" << maxRevertTokens << std::endl;

    if (isEmpty()) {
        std::cout << "No actions recorded." << std::endl;
    } else {
        std::cout << "Top action: " << stack->peek().description << std::endl;
    }
    std::cout << "==============================" << std::endl;
}

// Display status
void ActionStack::displayStatus() const {
    std::cout << "Action Stack Status:" << std::endl;
    std::cout << "  Size: " << stack->getSize() << std::endl;
    std::cout << "  Revert Tokens: " << revertTokens << "/" << maxRevertTokens << std::endl;
    if (!isEmpty()) {
        std::cout << "  Last Action: " << stack->peek().description << std::endl;
    }
}

// Clear all actions
void ActionStack::clear() {
    stack->clear();
    std::cout << "[ActionStack] All actions cleared." << std::endl;
}
