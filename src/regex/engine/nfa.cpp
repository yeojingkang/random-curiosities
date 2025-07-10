#include "nfa.h"

#include <stack>
#include <ranges>
#include <iostream>

namespace Regex
{
    constexpr auto EPSILON_CHAR = '\0';

    NFA::NFA()
    {
        auto start = std::make_unique<State>();
        auto accepting = std::make_unique<State>(true);

        startState = start.get();
        acceptingState = accepting.get();

        states.emplace_back(std::move(start));
        states.emplace_back(std::move(accepting));
    }

    void NFA::AcquireStatesFrom(NFA &other)
    {
        for (auto &state : other.states)
            states.emplace_back(std::move(state));
        other.states.clear();
    }

    NFA NFA::MakeEpsilon()
    {
        NFA nfa;
        nfa.startState->transitions.emplace_back(EPSILON_CHAR, nfa.acceptingState);
        return nfa;
    }

    NFA NFA::MakeChar(char c)
    {
        NFA nfa;
        nfa.startState->transitions.emplace_back(c, nfa.acceptingState);
        return nfa;
    }

    NFA NFA::MakeUnion(NFA &&nfa1, NFA &&nfa2)
    {
        NFA nfa;
        nfa.startState->transitions.emplace_back(EPSILON_CHAR, nfa1.startState);
        nfa.startState->transitions.emplace_back(EPSILON_CHAR, nfa2.startState);
        nfa1.acceptingState->transitions.emplace_back(EPSILON_CHAR, nfa.acceptingState);
        nfa2.acceptingState->transitions.emplace_back(EPSILON_CHAR, nfa.acceptingState);
        nfa1.acceptingState->isAccepting = false;
        nfa2.acceptingState->isAccepting = false;
        nfa.AcquireStatesFrom(nfa1);
        nfa.AcquireStatesFrom(nfa2);
        return nfa;
    }

    NFA NFA::MakeConcat(NFA &&nfa1, NFA &&nfa2)
    {
        NFA nfa;
        nfa1.acceptingState->transitions.emplace_back(EPSILON_CHAR, nfa2.startState);
        nfa1.acceptingState->isAccepting = false;
        nfa.startState = nfa1.startState;
        nfa.acceptingState = nfa2.acceptingState;
        nfa.AcquireStatesFrom(nfa1);
        nfa.AcquireStatesFrom(nfa2);
        return nfa;
    }

    NFA NFA::MakeKleeneStar(NFA &&nfa)
    {
        NFA newNFA;
        newNFA.startState->transitions.emplace_back(EPSILON_CHAR, nfa.startState);
        newNFA.startState->transitions.emplace_back(EPSILON_CHAR, newNFA.acceptingState);
        nfa.acceptingState->transitions.emplace_back(EPSILON_CHAR, nfa.startState);
        nfa.acceptingState->transitions.emplace_back(EPSILON_CHAR, newNFA.acceptingState);
        nfa.acceptingState->isAccepting = false;
        newNFA.AcquireStatesFrom(nfa);
        return newNFA;
    }

    std::set<State *> NFA::EpsilonClosure(const std::set<State *> &states)
    {
        std::stack<State *> currStack;
        std::set<State *> result = states;

        auto isEpsilonAndNotInResult = [&result](const Transition &t)
        {
            return t.first == EPSILON_CHAR && !result.contains(t.second);
        };

        for (const auto s : states)
            currStack.push(s);

        while (!currStack.empty()) {
            const auto s = currStack.top();
            currStack.pop();

            for (const auto &et : s->transitions
                                | std::views::filter(isEpsilonAndNotInResult))
            {
                result.emplace(et.second);
                currStack.push(et.second);
            }
        }

        return result;
    }

    std::set<State *> NFA::Move(const std::set<State *> &states, char c)
    {
        std::set<State *> result;
        auto hasTransition = [c](const Transition &t) { return t.first == c; };

        for (const auto s : states)
        {
            for (const auto &t : s->transitions | std::views::filter(hasTransition))
                result.emplace(t.second);
        }

        return result;
    }

    void NFA::Print() const
    {
        std::cout << "Start state: " << startState << std::endl
            << "Accepting state: " << acceptingState << std::endl;

        for (const auto &s : states)
        {
            std::cout << s.get() << std::endl
                << "\tIsAccepting: " << s->isAccepting << std::endl;
            std::cout << "\tTransitions:" << std::endl;

            for (const auto &t : s->transitions)
            {
                const auto &[c, s] = t;
                std::cout << '\t';
                if (c == EPSILON_CHAR)
                    std::cout << "ep";
                else
                    std::cout << c;

                std::cout << " -> " << s << std::endl;
            }

            std::cout << std::endl;
        }
    }
}
