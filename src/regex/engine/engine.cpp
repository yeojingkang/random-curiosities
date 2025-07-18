#include "engine.h"

#include <regex>
#include <iostream>

namespace Regex
{
    void Engine::Compile()
    {
        pos = 0;
        nfa = ParseExpr();
        nfa.Print();
    }

    bool Engine::Matches(std::string_view input)
    {
        auto currStates = NFA::EpsilonClosure({nfa.startState});

        for (const auto c : input)
        {
            currStates = NFA::EpsilonClosure(NFA::Move(currStates, c));
            if (currStates.empty())
                return false;
        }

        for (const auto s : currStates)
            if (s->isAccepting)
                return true;

        return false;
    }

    NFA Engine::ParseExpr() { return ParseUnion(); }

    NFA Engine::ParseUnion()
    {
        auto result = ParseConcat();
        while (!IsAtEnd() && Peek() == '|')
        {
            ++pos;
            auto rUnion = ParseConcat();
            result = NFA::MakeUnion(std::move(result), std::move(rUnion));
        }

        return result;
    }

    NFA Engine::ParseConcat()
    {
        auto result = ParseDuplication();
        while (!IsAtEnd() && Peek() != '|' && Peek() != ')')
        {
            auto concatNFA = ParseDuplication();
            result = NFA::MakeConcat(std::move(result), std::move(concatNFA));
        }

        return result;
    }

    NFA Engine::ParseDuplication()
    {
        auto result = ParseAtom();
        while (!IsAtEnd())
        {
            const auto c = Peek();
            const auto makeNFA =
                c == '*' ? NFA::MakeKleeneStar :
                c == '+' ? NFA::MakePlus :
                nullptr;

            if (makeNFA == nullptr)
                break;

            ++pos;
            result = makeNFA(std::move(result));
        }

        return result;
    }

    NFA Engine::ParseAtom()
    {
        if (IsAtEnd())
            return NFA::MakeEpsilon();

        const auto currChar = Advance();

        if (currChar != '(')
            return NFA::MakeChar(currChar); // Read single char

        // Capture group
        auto result = ParseExpr();

        if (!IsAtEnd() && Peek() == ')')
            ++pos;
        else
            throw std::regex_error{std::regex_constants::error_paren}; // TODO: Throw something proper

        return result;
    }

    char Engine::Advance() { return pattern[pos++]; }
    char Engine::Peek() const { return pattern[pos]; }
    bool Engine::IsAtEnd() const { return pos >= static_cast<int>(pattern.length()); }
}
