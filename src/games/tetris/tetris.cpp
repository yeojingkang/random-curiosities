#include "tetris.h"

#include <algorithm>

namespace Tetris
{
    Board::Board() :
        rng{ std::random_device{}() },
        rngDistrib{ 0, static_cast<UniformDistribution::result_type>(TetraPiece::NUM_VALUES) - 1, },
        score{ 0 },
        player{ SpawnPiece() }
    {
        Wipe(); // This is fine since default init for board does nothing
    }

    void Board::Step()
    {
        if (!MovePlayer(Pos{ player.pos.x, player.pos.y + 1 }))
        {
            CommitPiece();
            PerformScoring();
            player = SpawnPiece();
        }
    }

    void Board::PrintBoard(WINDOW *win) const
    {
        // Render board
        for (auto y = 0; y < HEIGHT; ++y)
            for (auto x = 0; x < WIDTH; ++x)
                mvwaddch(win, y + 1, x + 1, board[y][x]);

        // Render player tetramino
        const auto &playerTetra = TETRAMINOES[static_cast<int>(player.piece)][player.rotation];
        for (auto tilePos : playerTetra)
            mvwaddch(win, player.pos.y + tilePos.y + 1, player.pos.x + tilePos.x + 1, BLOCK_CHAR);
    }

    bool Board::GameOver() const
    {
        // TODO: Implement
        return false;
    }

    unsigned Board::Score() const { return score; }

    void Board::MovePlayerLeft(int n) { MovePlayer({ player.pos.x - n, player.pos.y }); }
    void Board::MovePlayerRight(int n) { MovePlayer({ player.pos.x + n, player.pos.y }); }
    void Board::MovePlayerDown(int n) { MovePlayer({ player.pos.x, player.pos.y + n }); }

    void Board::RotatePlayerCW() { RotatePlayer((player.rotation + 1) % 4); }
    void Board::RotatePlayerCCW() { RotatePlayer((player.rotation - 1 + 4) % 4); }

    inline void Board::Wipe()
    {
        for (auto &row : board)
            row.fill(' ');
    }

    void Board::CommitPiece()
    {
        const auto &tetra = TETRAMINOES[static_cast<int>(player.piece)][player.rotation];
        for (auto tilePos : tetra)
        {
            const auto absPos = player.pos + tilePos;
            board[absPos.y][absPos.x] = BLOCK_CHAR;
        }
    }

    inline PlayerTetra Board::SpawnPiece()
    {
        return { static_cast<TetraPiece>(rngDistrib(rng)), 0, SPAWN };
    }

    void Board::PerformScoring()
    {
        for (auto i = player.pos.y + 3; i >= player.pos.y; --i)
        {
            if (board[i] != FULL_ROW)
                continue;

            board[i].fill(' ');
            const auto rowIt = std::begin(board) + i;
            // TODO: Maybe multiple filled rows can be batch-rotated?
            std::rotate(std::begin(board), rowIt, rowIt + 1);
            ++i; // Stay on this row since rows above have shifted down
            ++score;
        }
    }

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

    bool Board::Collides(const PlayerTetra &potentialPlayer) const
    {
        const auto &tetra = TETRAMINOES[static_cast<int>(potentialPlayer.piece)][potentialPlayer.rotation];

        return std::any_of(std::begin(tetra), std::end(tetra),
            [this, &potentialPlayer](const auto &tilePos)
            {
                const auto absPos = potentialPlayer.pos + tilePos;

                return absPos.x < 0 || absPos.x >= WIDTH
                    || absPos.y < 0 || absPos.y >= HEIGHT
                    || board[absPos.y][absPos.x] == BLOCK_CHAR;
            });
    }
}
