#include "engine.h"

namespace Regex
{
    void Engine::Compile()
    {
        pos = 0;
        nfa = ParseExpr();
    }

    bool Engine::Matches(std::string_view input)
    {
        auto currStates = NFA::EpsilonClosure({nfa.startState});

        for (const auto c : input)
        {
            currStates = NFA::Move(currStates, c);
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
            result = NFA::MakeUnion(result, rUnion);
        }

        return result;
    }

    NFA Engine::ParseConcat()
    {
        auto result = ParseStar();
        while (!IsAtEnd() && Peek() != '|' && Peek() != ')')
        {
            ++pos;
            auto concatNFA = ParseStar();
            result = NFA::MakeConcat(result, concatNFA);
        }

        return result;
    }

    NFA Engine::ParseStar()
    {
        auto result = ParseAtom();
        while (!IsAtEnd() && Peek() == '*')
        {
            ++pos;
            result = NFA::MakeKleeneStar(result);
        }

        return result;
    }

    NFA Engine::ParseAtom()
    {
        if (pos > static_cast<int>(pattern.length()))
            return NFA::MakeEpsilon();

        const auto currChar = Advance();

        if (currChar != '(')
            return NFA::MakeChar(currChar); // Read single char

        // Capture group
        const auto result = ParseExpr();

        if (!IsAtEnd() && Peek() == ')')
            ++pos;
        else
            throw; // TODO: Throw something proper

        return result;
    }

    char Engine::Advance() { return pattern[pos++]; }
    char Engine::Peek() const { return pattern[pos]; }
    bool Engine::IsAtEnd() const { return pos < static_cast<int>(pattern.length()); }
}
