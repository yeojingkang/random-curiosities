#pragma once

#include <unordered_map>
#include <unordered_set>

namespace Regex
{
    struct State
    {
    public:
        using States = std::unordered_set<State *>;

        States epsilonTransitions;
        std::unordered_map<char, States> transitions;
        bool isAccepting;


        State(bool accepting = false) : isAccepting{ accepting } {}

        void AddEpsilonTransition(State *nextState);
        void AddTransition(char c, State *nextState);
    };
}
