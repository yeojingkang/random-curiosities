#include "tetris.h"

namespace Tetris
{
    Board::Board() :
        score{ 0 },
        player{TetraPiece::I, 0, SPAWN_X, SPAWN_Y }
    {
        Wipe(); // This is fine since default init for board does nothing
    }

    void Board::Step()
    {
        // TODO: Implement
    }

    void Board::PrintBoard(WINDOW *win) const
    {
        // Render board
        for (auto y = 0; y < HEIGHT; ++y)
            for (auto x = 0; x < WIDTH; ++x)
                mvwaddch(win, y + 1, x + 1, board[y * WIDTH + x]);

        // Render player tetramino
        const auto &playerTetra = TETRAMINOES[static_cast<int>(player.piece)][player.rotation];
        for (auto idx : playerTetra)
        {
            mvwaddch(win, player.y + idx.y + 1, player.x + idx.x + 1, BLOCK_CHAR);
        }
    }

    bool Board::GameOver() const
    {
        // TODO: Implement
        return false;
    }

    unsigned Board::Score() const { return score; }

    inline void Board::Wipe()
    {
        board.fill(' ');
    }
}
