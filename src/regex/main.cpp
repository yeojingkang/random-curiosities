#include <iostream>

#include "engine/engine.h"

int main()
{
    std::string regex, input;

    std::cout << "Regex: ";
    std::cin >> regex;
    std::cout << "Input string: ";
    std::cin >> input;

    Regex::Engine engine{regex};
    engine.Compile();
    std::cout << engine.Matches(input) << std::endl;
}
