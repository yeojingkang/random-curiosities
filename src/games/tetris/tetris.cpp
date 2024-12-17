#include "tetris.h"

#include <algorithm>

namespace Tetris
{
    Pos operator+(const Pos &lhs, const Pos &rhs)
    {
        return Pos{ lhs.x + rhs.x, lhs.y + rhs.y };
    }

    Board::Board() :
        score{ 0 },
        player{ TetraPiece::I, 0, SPAWN_X, SPAWN_Y }
    {
        Wipe(); // This is fine since default init for board does nothing
    }

    void Board::Step()
    {
        MovePlayer(Pos{ player.pos.x, player.pos.y + 1 });
        // TODO: If cannot move player, lock in piece & do scoring, spawn next piece
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
            mvwaddch(win, player.pos.y + idx.y + 1, player.pos.x + idx.x + 1, BLOCK_CHAR);
        }
    }

    bool Board::GameOver() const
    {
        // TODO: Implement
        return false;
    }

    unsigned Board::Score() const { return score; }

    bool Board::MovePlayer(Pos pos)
    {
        auto updatedPlayer = player;
        updatedPlayer.pos = pos;

        return TryUpdatePlayer(updatedPlayer);
    }

    bool Board::RotatePlayer(int rot)
    {
        auto updatedPlayer = player;
        updatedPlayer.rotation = rot;

        /*
         * NOTE: This is just a demonstration to show that TryUpdatePlayer
         *       supports const/non-const lvalue/rvalue
         */
        /*
        const auto f = updatedPlayer;
        TryUpdatePlayer(f);
        TryUpdatePlayer(std::move(f));
        TryUpdatePlayer(PlayerTetra{ TetraPiece::I, 0, SPAWN_X, SPAWN_Y });
        */

        return TryUpdatePlayer(updatedPlayer);
    }

    inline void Board::Wipe()
    {
        board.fill(' ');
    }

    bool Board::Collides(const PlayerTetra &potentialPlayer) const
    {
        const auto &tetra = TETRAMINOES[static_cast<int>(potentialPlayer.piece)][potentialPlayer.rotation];

        return std::any_of(std::begin(tetra), std::end(tetra),
            [this, &potentialPlayer](const auto &tilePos)
            {
                const auto absPos = potentialPlayer.pos + tilePos;

                return absPos.x < 0 || absPos.x >= WIDTH
                    || absPos.y < 0 || absPos.y >= HEIGHT
                    || board[absPos.y * WIDTH + absPos.x] == BLOCK_CHAR;
            });
    }
}
