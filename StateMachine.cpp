#include "StateMachine.h"

void StateMachine::addState(StateRef _newState, bool _isReplacing)
{
    this->isAdding = true;
    this->isReplacing = _isReplacing;

    this->newState = std::move(_newState);
}

void StateMachine::removeState()
{
    this->isRemoving = true;
}

void StateMachine::processStateChanges()
{
    if (this->isRemoving && !this->states.empty())
    {
        this->states.pop();

        if (!this->states.empty())
        {
            this->states.top()->resume();
        }

        this->isRemoving = false;
    }

    if (this->isAdding)
    {
        if (!this->states.empty())
        {
            if (this->isReplacing)
            {
                this->states.pop();
            }
            else
            {
                this->states.top()->pause();
            }
        }

        this->states.push(std::move(this->newState));
        this->states.top()->init();
        this->isAdding = false;
    }
}

StateRef& StateMachine::getActiveState()
{
    return this->states.top();
}