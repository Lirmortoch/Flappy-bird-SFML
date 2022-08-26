#pragma once

#include <stack>
#include <memory>

#include "State.h"

typedef std::unique_ptr <State> StateRef;

class StateMachine
{
private:
    std::stack <StateRef> states;
    StateRef newState;

    bool isRemoving;
    bool isAdding;
    bool isReplacing;
public:
    StateMachine() {}

    void addState(StateRef _newState, bool _isReplacing = true);

    void removeState();
   
    void processStateChanges();
    
    StateRef& getActiveState();

    ~StateMachine() {}
};