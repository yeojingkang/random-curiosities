#pragma once

#include <array>

namespace Tetris
{
    enum class TetraPiece : int
    {
        I = 0,
        O, J, L, Z, S, T,
        NUM_VALUES
    };

    struct Pos
    {
        int x, y;
    };

    inline Pos operator+(const Pos &lhs, const Pos &rhs)
    {
        return Pos{ lhs.x + rhs.x, lhs.y + rhs.y };
    }

    using Tetramino = std::array<Pos, 4>;
    using TetraRot = std::array<Tetramino, 4>;

    // The complete list of every piece's rotations
    inline constexpr std::array<TetraRot, 7> TETRAMINOES = {
        TetraRot{ // I
            Tetramino{ Pos{ 0, 1 }, Pos{ 1, 1 }, Pos{ 2, 1 }, Pos{ 3, 1 } },
            Tetramino{ Pos{ 2, 0 }, Pos{ 2, 1 }, Pos{ 2, 2 }, Pos{ 2, 3 } },
            Tetramino{ Pos{ 0, 2 }, Pos{ 1, 2 }, Pos{ 2, 2 }, Pos{ 3, 2 } },
            Tetramino{ Pos{ 1, 0 }, Pos{ 1, 1 }, Pos{ 1, 2 }, Pos{ 1, 3 } },
        },
        TetraRot{ // O
            Tetramino{ Pos{ 1, 0 }, Pos{ 2, 0 }, Pos{ 1, 1 }, Pos{ 2, 1 } },
            Tetramino{ Pos{ 1, 0 }, Pos{ 2, 0 }, Pos{ 1, 1 }, Pos{ 2, 1 } },
            Tetramino{ Pos{ 1, 0 }, Pos{ 2, 0 }, Pos{ 1, 1 }, Pos{ 2, 1 } },
            Tetramino{ Pos{ 1, 0 }, Pos{ 2, 0 }, Pos{ 1, 1 }, Pos{ 2, 1 } },
        },
        TetraRot{ // J
            Tetramino{ Pos{ 0, 0 }, Pos{ 0, 1 }, Pos{ 1, 1 }, Pos{ 2, 1 } },
            Tetramino{ Pos{ 1, 0 }, Pos{ 2, 0 }, Pos{ 1, 1 }, Pos{ 1, 2 } },
            Tetramino{ Pos{ 0, 1 }, Pos{ 1, 1 }, Pos{ 2, 1 }, Pos{ 2, 2 } },
            Tetramino{ Pos{ 1, 0 }, Pos{ 1, 1 }, Pos{ 0, 2 }, Pos{ 1, 2 } },
        },
        TetraRot{ // L
            Tetramino{ Pos{ 2, 0 }, Pos{ 0, 1 }, Pos{ 1, 1 }, Pos{ 2, 1 } },
            Tetramino{ Pos{ 1, 0 }, Pos{ 1, 1 }, Pos{ 1, 2 }, Pos{ 2, 2 } },
            Tetramino{ Pos{ 0, 1 }, Pos{ 1, 1 }, Pos{ 2, 1 }, Pos{ 0, 2 } },
            Tetramino{ Pos{ 0, 0 }, Pos{ 1, 0 }, Pos{ 1, 1 }, Pos{ 1, 2 } },
        },
        TetraRot{ // Z
            Tetramino{ Pos{ 0, 0 }, Pos{ 1, 0 }, Pos{ 1, 1 }, Pos{ 2, 1 } },
            Tetramino{ Pos{ 2, 0 }, Pos{ 1, 1 }, Pos{ 2, 1 }, Pos{ 1, 2 } },
            Tetramino{ Pos{ 0, 1 }, Pos{ 1, 1 }, Pos{ 1, 2 }, Pos{ 2, 2 } },
            Tetramino{ Pos{ 1, 0 }, Pos{ 0, 1 }, Pos{ 1, 1 }, Pos{ 0, 2 } },
        },
        TetraRot{ // S
            Tetramino{ Pos{ 1, 0 }, Pos{ 2, 0 }, Pos{ 0, 1 }, Pos{ 1, 1 } },
            Tetramino{ Pos{ 1, 0 }, Pos{ 1, 1 }, Pos{ 2, 1 }, Pos{ 2, 2 } },
            Tetramino{ Pos{ 1, 1 }, Pos{ 2, 1 }, Pos{ 0, 2 }, Pos{ 1, 2 } },
            Tetramino{ Pos{ 0, 0 }, Pos{ 0, 1 }, Pos{ 1, 1 }, Pos{ 1, 2 } },
        },
        TetraRot{ // T
            Tetramino{ Pos{ 1, 0 }, Pos{ 0, 1 }, Pos{ 1, 1 }, Pos{ 2, 1 } },
            Tetramino{ Pos{ 1, 0 }, Pos{ 1, 1 }, Pos{ 2, 1 }, Pos{ 1, 2 } },
            Tetramino{ Pos{ 0, 1 }, Pos{ 1, 1 }, Pos{ 2, 1 }, Pos{ 1, 2 } },
            Tetramino{ Pos{ 1, 0 }, Pos{ 0, 1 }, Pos{ 1, 1 }, Pos{ 1, 2 } },
        },
    };
}
