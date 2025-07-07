#pragma once

#include <string>
#include <string_view>

#include "nfa.h"

namespace Regex
{
    class Engine
    {
    public:
        Engine(const std::string &regex) : pattern{regex}, pos{0} {}

        void Compile();
        bool Matches(std::string_view input);

    private:
        const std::string pattern;
        int pos;
        NFA nfa;

        NFA ParseExpr();
        NFA ParseUnion();
        NFA ParseConcat();
        NFA ParseStar();
        NFA ParseAtom();

        char Advance();
        char Peek() const;
        bool IsAtEnd() const;
    };
}
