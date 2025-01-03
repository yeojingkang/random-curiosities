#pragma once

#include <array>
#include <concepts>
#include <random>
#include <ncurses.h>

#include "tetramino.h"

namespace Tetris
{
    inline constexpr auto WIDTH = 10;
    inline constexpr auto HEIGHT = 20;

    using Row = std::array<char, WIDTH>;
    using Board = std::array<Row, HEIGHT>;

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
    class Instance
    {
    public:

        Instance();

        void Step();
        void PrintBoard(WINDOW *win) const;
        bool GameOver() const;
        unsigned Score() const;

        void MovePlayerLeft(int n = 1);
        void MovePlayerRight(int n = 1);
        void MovePlayerDown(int n = 1);

        void RotatePlayerCW();
        void RotatePlayerCCW();

    private:
        // TODO: Maybe move RNG stuffs to their own class/file
        using RandNumGen = std::minstd_rand;
        using UniformDistribution = std::uniform_int_distribution<RandNumGen::result_type>;
        RandNumGen rng;
        UniformDistribution rngDistrib;

        Board board;
        unsigned score;
        PlayerTetra player;
        int ghostY;

        bool gameOver;

        // TODO: Convert functions here to free functions if possible
        void Wipe();
        void CommitPiece();
        PlayerTetra SpawnPiece();
        void PerformScoring();

        bool MovePlayer(Pos pos);
        bool RotatePlayer(int rot);

        bool TryUpdatePlayer(const PlayerTetra &updatedPlayer);
    };
}
