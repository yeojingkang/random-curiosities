#include "tetris.h"

#include <algorithm>

namespace Tetris
{
    Instance::Instance() :
        rng{ std::random_device{}() },
        rngDistrib{ 0, static_cast<UniformDistribution::result_type>(TetraPiece::NUM_VALUES) - 1 },
        score{ 0 },
        player{ SpawnPiece() },
        gameOver{ false }
    {
        Wipe(); // This is fine since default init for board does nothing
    }

    void Instance::Step()
    {
        if (gameOver)
            return;

        if (!MovePlayer(Pos{ player.pos.x, player.pos.y + 1 }))
        {
            CommitPiece();
            PerformScoring();
            player = SpawnPiece();

            if (Collides(player))
                gameOver = true;
        }
    }

    void Instance::PrintBoard(WINDOW *win) const
    {
        // Render board
        for (auto y = 0; y < HEIGHT; ++y)
            for (auto x = 0; x < WIDTH; ++x)
                mvwaddch(win, y + 1, x + 1, board[y][x]);

        const auto &playerTetra = TETRAMINOES[static_cast<int>(player.piece)][player.rotation];

        // Render ghost
        wattrset(win, A_DIM);
        for (auto tilePos : playerTetra)
            mvwaddch(win, ghostY + tilePos.y + 1, player.pos.x + tilePos.x + 1, BLOCK_CHAR);
        wstandend(win);

        // Render player tetramino
        for (auto tilePos : playerTetra)
            mvwaddch(win, player.pos.y + tilePos.y + 1, player.pos.x + tilePos.x + 1, BLOCK_CHAR);
    }

    bool Instance::GameOver() const { return gameOver; }
    unsigned Instance::Score() const { return score; }

    void Instance::MovePlayerLeft(int n) { MovePlayer({ player.pos.x - n, player.pos.y }); }
    void Instance::MovePlayerRight(int n) { MovePlayer({ player.pos.x + n, player.pos.y }); }
    void Instance::MovePlayerDown(int n) { MovePlayer({ player.pos.x, player.pos.y + n }); }

    void Instance::RotatePlayerCW() { RotatePlayer((player.rotation + 1) % 4); }
    void Instance::RotatePlayerCCW() { RotatePlayer((player.rotation - 1 + 4) % 4); }

    inline void Instance::Wipe()
    {
        for (auto &row : board)
            row.fill(' ');
    }

    void Instance::CommitPiece()
    {
        const auto &tetra = TETRAMINOES[static_cast<int>(player.piece)][player.rotation];
        for (auto tilePos : tetra)
        {
            const auto absPos = player.pos + tilePos;
            board[absPos.y][absPos.x] = BLOCK_CHAR;
        }
    }

    inline PlayerTetra Instance::SpawnPiece()
    {
        auto newPiece = PlayerTetra{ static_cast<TetraPiece>(rngDistrib(rng)), 0, SPAWN };
        ghostY = CalcGhostY(newPiece);
        return newPiece;
    }

    void Instance::PerformScoring()
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

    bool Instance::MovePlayer(Pos pos)
    {
        auto updatedPlayer = player;
        updatedPlayer.pos = pos;

        return TryUpdatePlayer(updatedPlayer);
    }

    bool Instance::RotatePlayer(int rot)
    {
        auto updatedPlayer = player;
        updatedPlayer.rotation = rot;

        return TryUpdatePlayer(updatedPlayer);
    }

    int Instance::CalcGhostY(PlayerTetra tetra) const
    {
        // Rare do-while loop?
        do
        {
            ++tetra.pos.y;
        } while (!Collides(tetra));

        return tetra.pos.y - 1;
    }

    bool Instance::TryUpdatePlayer(const PlayerTetra &updatedPlayer)
    {
        const auto collides = Collides(updatedPlayer);
        if (!collides)
        {
            if (player.pos.x != updatedPlayer.pos.x ||
                player.rotation != updatedPlayer.rotation)
                ghostY = CalcGhostY(updatedPlayer);
            player = updatedPlayer;
        }

        return !collides;
    }

    bool Instance::Collides(const PlayerTetra &potentialPlayer) const
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
