#pragma once

#include <vector>
#include <set>
#include <memory>

namespace Regex
{
    struct State;

    // Epsilon transitions are defined by { '\0', nextState }
    using Transition = std::pair<char, State *>;

    struct State
    {
    public:
        std::vector<Transition> transitions;
        bool isAccepting;

        State(bool accepting = false) : isAccepting{ accepting } {}
    };

    class NFA
    {
    public:
        NFA();

        // NFAs cannot be copied
        NFA(const NFA&) = delete;
        NFA& operator=(const NFA&) = delete;

        NFA(NFA&&) = default;
        NFA& operator=(NFA&&) = default;

        static NFA MakeEpsilon();
        static NFA MakeChar(char c);
        static NFA MakeUnion(NFA &&nfa1, NFA &&nfa2);
        static NFA MakeConcat(NFA &&nfa1, NFA &&nfa2);
        static NFA MakeKleeneStar(NFA &&nfa);

        static std::set<State *> EpsilonClosure(const std::set<State *> &states);
        static std::set<State *> Move(const std::set<State *> &states, char c);

        void Print() const;

        State *startState;
        State *acceptingState;

    private:
        std::vector<std::unique_ptr<State>> states;

        void AcquireStatesFrom(NFA &other);
    };
}
