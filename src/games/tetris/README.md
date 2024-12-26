# Tetris
Tetris, at its core, is a simple game. On a 10x20 board, the player has to control falling blocks and form rows, which will then be removed from the board and score points. The game is over if the blocks stack up too high and block the next player piece from spawning.

However, developing the game isn't a trivial feat. There are various mechanics whose implementation isn't immediately clear to me.

## Tetramino
Tetraminoes are contiguous pieces of 4 blocks/tiles, and the player shifts them left and right and/or rotate them as they fall to the bottom of the board. Developing the controls and movement of these tetraminoes confounded me as I couldn't relate a tetramino's core position to its tiles whilst taking rotation logic into account, though once I realised a few points, the problem became simpler and clearer.

- Every tetramino (and its rotations) can be contained in a 4x4 mini-board
- Tetramino rotation positions are fixed and always the same in the mini-board

With these points in mind, I developed the tetraminoes as follows:
- A tetramino is a 4x4 mini-board containing its tiles
```cpp
using Tetramino = std::array<Pos, 4>; // Coordinates are local to the mini-board
```
- The mini-board's position is its top-left corner, which can move outside of the board as long as its tiles remain inside
- All tetramino rotations are hardcoded and placed in an array (in a rotation-friendly sequence so rotations simply cycle through the array)
```cpp
using TetraRot = std::array<Tetramino, 4>;

// The complete list of every piece's rotations
constexpr std::array<TetraRot, 7> TETRAMINOES = {
    TetraRot{ // I
        Tetramino{ Pos{ 0, 1 }, Pos{ 1, 1 }, Pos{ 2, 1 }, Pos{ 3, 1 } },
        Tetramino{ Pos{ 2, 0 }, Pos{ 2, 1 }, Pos{ 2, 2 }, Pos{ 2, 3 } },
        Tetramino{ Pos{ 0, 2 }, Pos{ 1, 2 }, Pos{ 2, 2 }, Pos{ 3, 2 } },
        Tetramino{ Pos{ 1, 0 }, Pos{ 1, 1 }, Pos{ 1, 2 }, Pos{ 1, 3 } },
    },
    // TetraRots of other pieces...
};
```

## The player and the board
A naive way to implement the player and the board is to put the player's tetramino in the board and move it as necessary. One problem with this approach is that it's unclear which tiles on the board belong to the player, and movement + collision check becomes unnecessarily complex. For example, if we want to move the vertical I piece down 1 tile, we must first check the new position for collision, which would require keeping track of the player's tiles on the board.

Instead, we can separate the player and the board into different layers. The player "hovers" in front of the board (whilst respecting collision rules), and is only added to the board if it can't drop any further.

### Collision
This is a trivial matter of checking each player tile against the board for overlaps.

## Board rows
My initial implementation of the board was a single array of chars, and the rows/columns were managed arithmetically. This made row management unnecessarily complex, especially row shifting. I'd have to manually `memcpy` elements down the array, making sure not to overwrite data that weren't moved yet.

I then changed my implementation to an `std::array` of rows (each an `std::array` of chars). Coupled with `std::rotate`, this made line clearing and shifting simple. I still have to measure the performance of this approach.

## TODO
- Check if manual management of board through single array is more performant
- [Super Rotation System](https://tetris.wiki/Super_Rotation_System)
- [Random Bag](https://tetris.wiki/Random_Generator)
- Hold/swap mechanic
- Hard drop
