#include "tetris.h"

Tetris::Tetris()
{
    WipeBoard(); // This is fine since default init for board does nothing
}

void Tetris::Step()
{
    // TODO: Implement
}

void Tetris::PrintBoard(WINDOW *win) const
{
    for (auto y = 0; y < HEIGHT; ++y)
        for (auto x = 0; x < WIDTH; ++x)
            mvwaddch(win, y + 1, x + 1, board[y * WIDTH + x]);
}

bool Tetris::GameOver() const
{
    // TODO: Implement
    return false;
}

unsigned Tetris::Score() const { return score; }

inline void Tetris::WipeBoard()
{
    board.fill(' ');
}
