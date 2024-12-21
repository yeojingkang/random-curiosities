# Tetris
Tetris, at its core, is a simple game. On a 10x20 board, the player has to control falling blocks and form rows, which will then be removed from the board and score points. The game is over if the blocks stack up too high and block the next player piece from spawning.

However, developing the game isn't a trivial feat. There are various mechanics that make up the game's logic.

## Tetramino
Tetraminoes are contiguous pieces of 4 blocks/tiles, and the player shifts them left and right and/or rotate them as they fall to the bottom of the board. Developing the controls and movement of these tetraminoes confounded me as I couldn't relate a tetramino's core position to its tiles whilst taking rotation logic into account, though once I realised a few points, the problem became simpler and clearer.

- Every tetramino (and its rotations) can be contained in a 4x4 mini-board
- Tetramino rotation positions are fixed and always the same in the mini-board

With these points in mind, I developed the tetraminoes as follows:
- A tetramino is a 4x4 mini-board containing its tiles
- The mini-board's position is its top-left corner, which can move outside of the board as long as its tiles remain inside
- All tetramino rotations are hardcoded and placed in an array (in a rotation-friendly sequence so rotations simply cycle through the array)

## The player and the board
A naive way to implement the player and the board is to put the player's tetramino in the board and move it as necessary. One problem with this approach is that it's unclear which tiles on the board belong to the player, and the board's tiles might accidentally be moved.

Instead, we can think of the player and the board as separate entities on different layers, like a magnet and a fridge. The player "hovers" in front of the board, and is only added to the board if it can't drop any further.

### Collision
This is a trivial matter of checking each player tile against the board for overlaps.

## Board rows
My initial implementation of the board was a single array of chars, and the rows/columns were managed arithmetically. This made row management unnecessarily complex, especially row shifting. I'd have to manually `memcpy` elements down the array, making sure not to overwrite data that weren't moved yet.

I then changed my implementation to an `std::array` of rows (each an `std::array` of chars). Coupled with `std::rotate`, this made row clearing and shifting simple. I still have to measure the performance of this approach.

## TODO
- Check if manual management of board through single array is more performant
- [Super Rotation System](https://tetris.wiki/Super_Rotation_System)
- [Random Bag](https://tetris.wiki/Random_Generator)
- Hold/swap mechanic
- Hard drop
