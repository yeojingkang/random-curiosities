#include <iostream>

#include "tetris.h"

int main()
{
    Tetris tetris;

    std::cout << "This is a program that plays tetris" << std::endl;

    while (!tetris.GameOver())
    {
        tetris.Step();
        tetris.PrintBoard();
    }
}
