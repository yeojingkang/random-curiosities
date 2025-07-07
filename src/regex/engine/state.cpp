#include "state.h"

namespace Regex
{
    void State::AddEpsilonTransition(State *nextState)
    {
        epsilonTransitions.emplace(nextState);
    }

    void State::AddTransition(char c, State *nextState)
    {
        transitions[c].emplace(nextState);
    }
}
