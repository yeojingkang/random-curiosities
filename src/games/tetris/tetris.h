#pragma once

#include <array>
#include <ncurses.h>

#include "tetramino.h"

namespace Tetris
{
    struct PlayerTetra
    {
        TetraPiece piece;
        int rotation;
        int x, y;
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

    private:
        static constexpr auto BLOCK_CHAR = 'x';
        static constexpr auto SPAWN_X = 3;
        static constexpr auto SPAWN_Y = 0;

        std::array<char, WIDTH * HEIGHT> board;
        unsigned score;
        PlayerTetra player;

        void Wipe();
    };
}
