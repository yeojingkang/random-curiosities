#pragma once

#include <array>
#include <ncurses.h>

class Tetris
{
public:
    static constexpr int WIDTH = 10;
    static constexpr int HEIGHT = 20;

    Tetris();

    void Step();
    void PrintBoard(WINDOW *win) const;
    bool GameOver() const;
    unsigned Score() const;

private:
    using Tetramino = std::array<int, 4>;
    static constexpr std::array<Tetramino, 7> tetraminoes = {
        Tetramino{ 3,  4,  5,  6 }, // I
        Tetramino{ 4,  5, 14, 15 }, // O
        Tetramino{ 3, 13, 14, 15 }, // J
        Tetramino{ 5, 13, 14, 15 }, // L
        Tetramino{ 3,  4, 14, 15 }, // Z
        Tetramino{ 4,  5, 13, 14 }, // S
        Tetramino{ 4, 13, 14, 15 }  // T
    };

    std::array<char, WIDTH * HEIGHT> board;
    unsigned score = 0;

    void WipeBoard();
};
