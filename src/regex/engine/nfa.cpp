#include "nfa.h"

#include <stack>

namespace Regex
{
    NFA::NFA()
    {
        auto start = std::make_unique<State>();
        auto accepting = std::make_unique<State>(true);

        startState = start.get();
        acceptingState = accepting.get();

        states.emplace_back(std::move(start));
        states.emplace_back(std::move(accepting));
    }

    NFA NFA::MakeEpsilon()
    {
        NFA nfa;
        nfa.startState->AddEpsilonTransition(nfa.acceptingState);
        return nfa;
    }

    NFA NFA::MakeChar(char c)
    {
        NFA nfa;
        nfa.startState->AddTransition(c, nfa.acceptingState);
        return nfa;
    }

    NFA NFA::MakeUnion(NFA &nfa1, NFA &nfa2)
    {
        NFA nfa;
        nfa.startState->AddEpsilonTransition(nfa1.startState);
        nfa.startState->AddEpsilonTransition(nfa2.startState);
        nfa1.acceptingState->AddEpsilonTransition(nfa.acceptingState);
        nfa2.acceptingState->AddEpsilonTransition(nfa.acceptingState);
        nfa1.acceptingState->isAccepting = false;
        nfa2.acceptingState->isAccepting = false;
        nfa.AcquireStatesFrom(nfa1);
        nfa.AcquireStatesFrom(nfa2);
        return nfa;
    }

    NFA NFA::MakeConcat(NFA &nfa1, NFA &nfa2)
    {
        NFA nfa;
        nfa1.acceptingState->AddEpsilonTransition(nfa2.startState);
        nfa1.acceptingState->isAccepting = false;
        nfa.startState = nfa1.startState;
        nfa.acceptingState = nfa2.acceptingState;
        nfa.AcquireStatesFrom(nfa1);
        nfa.AcquireStatesFrom(nfa2);
        return nfa;
    }

    NFA NFA::MakeKleeneStar(NFA &nfa)
    {
        NFA newNFA;
        newNFA.startState->AddEpsilonTransition(nfa.startState);
        newNFA.startState->AddEpsilonTransition(newNFA.acceptingState);
        nfa.acceptingState->AddEpsilonTransition(nfa.startState);
        nfa.acceptingState->AddEpsilonTransition(newNFA.acceptingState);
        nfa.acceptingState->isAccepting = false;
        newNFA.AcquireStatesFrom(nfa);
        return newNFA;
    }

    std::unordered_set<State *> NFA::EpsilonClosure(const std::unordered_set<State *> &states)
    {
        std::stack<State *> currStack;
        std::unordered_set<State *> result;

        for (const auto s : states)
            currStack.push(s);

        while (!currStack.empty()) {
            const auto s = currStack.top();
            currStack.pop();

            for (const auto et : s->epsilonTransitions)
            {
                if (!result.contains(et))
                {
                    result.emplace(et);
                    currStack.push(et);
                }
            }
        }

        return result;
    }

    std::unordered_set<State *> NFA::Move(const std::unordered_set<State *> &states, char c)
    {
        for (const auto s : states)
        {
            if (s->transitions.contains(c))
                return s->transitions[c];
        }

        return {};
    }
}
