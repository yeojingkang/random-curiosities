#pragma once

#include <array>
#include <concepts>
#include <ncurses.h>

#include "tetramino.h"

namespace Tetris
{
    struct PlayerTetra
    {
        TetraPiece piece;
        int rotation;
        Pos pos;
    };

    /*
    * Contains everything needed for a Tetris board to function. It does not
    * keep track of time to determine when to update its state. (This is done
    * somewhere else)
    */
    class Board
    {
    public:
        static constexpr auto WIDTH = 10;
        static constexpr auto HEIGHT = 20;

        Board();

        void Step();
        void PrintBoard(WINDOW *win) const;
        bool GameOver() const;
        unsigned Score() const;

        bool MovePlayer(Pos pos);
        bool RotatePlayer(int rot);

    private:
        static constexpr auto BLOCK_CHAR = 'x';
        static constexpr auto SPAWN_X = 3;
        static constexpr auto SPAWN_Y = 0;

        std::array<char, WIDTH * HEIGHT> board;
        unsigned score;
        PlayerTetra player;

        void Wipe();

        template<typename T>
            requires std::same_as<std::remove_cvref_t<T>, PlayerTetra>
        bool TryUpdatePlayer(T &&updatedPlayer)
        {
            auto collides = Collides(updatedPlayer);
            if (!collides)
            {
                // Use move semantics where possible (i.e. non-const values)
                if constexpr (std::is_const_v<std::remove_reference_t<T>>)
                    player = std::forward<T>(updatedPlayer);
                else
                    std::swap(player, updatedPlayer);
            }

            return !collides;
        }

        bool Collides(const PlayerTetra &potentialPlayer) const;
    };
}
