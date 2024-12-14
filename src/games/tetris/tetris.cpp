#include "tetris.h"

#include <iostream>

Tetris::Tetris()
{
    WipeBoard(); // This is fine since default init for board does nothing
}

void Tetris::Step()
{
    // TODO: Implement
}

void Tetris::PrintBoard() const
{
    // TODO: Is there a more efficient way to do this?
    static constexpr auto SIZE = WIDTH * HEIGHT;

    std::cout << "|----------";

    for (auto i = 0; i < SIZE; ++i)
    {
        if (i % WIDTH == 0)
            std::cout << "|" << std::endl << "|";

        std::cout << board[i];
    }

    std::cout
        << "|" << std::endl
        << "|----------|" << std::endl;
}

bool Tetris::GameOver() const
{
    // TODO: Implement
    return false;
}

inline void Tetris::WipeBoard()
{
    board.fill(' ');
}
