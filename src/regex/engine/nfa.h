#pragma once

#include <vector>
#include <unordered_set>
#include <memory>

#include "state.h"

namespace Regex
{
    class NFA
    {
    public:
        NFA();

        void AcquireStatesFrom(NFA &other);

        static NFA MakeEpsilon();
        static NFA MakeChar(char c);
        static NFA MakeUnion(NFA &nfa1, NFA &nfa2);
        static NFA MakeConcat(NFA &nfa1, NFA &nfa2);
        static NFA MakeKleeneStar(NFA &nfa);

        static std::unordered_set<State *> EpsilonClosure(const std::unordered_set<State *> &states);
        static std::unordered_set<State *> Move(const std::unordered_set<State *> &states, char c);

        State *startState;
        State *acceptingState;

    private:
        std::vector<std::unique_ptr<State>> states;
    };
}
