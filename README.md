# Connect 4 Game Solver
Around 2 years ago, I wrote a Connect 4 bot in `C` solely using concepts that I learned in my introduction to programming class. Though it is impressive for my skills at that time, the bot I wrote was very inefficient and badly organized in a single `C` file. As such I would like to rewrite the A.I. in `C++` with concepts that I learnt from Data Structures and Algorithms, Object Oriented Programming and Computer organization.

## Acknowledgement
Before I describe what I learnt and implemented throughout this project, I would like to express my sincere gratitude to Pascal Pons. This project wouldn't be possible (and as efficient) without his tutorial on advanced bitboard manipulation and the optimized transposition table. I highly recommend you to take a look at his tutorial if you are intersted in making your own solver! There are many, many efficient implementations and techniques that I learnt throughout making this solver.

### Links to resources that I used
Pascal Pons: [Solving Connect 4: how to build a perfect AI](http://blog.gamesolver.org/)\
ingank: [Bitboards and Connect Four](https://github.com/denkspuren/BitboardC4/blob/master/BitboardDesign.md)\
Gilles Vandewiele: [Creating the (nearly) perfect connect-four bot with limited move time and file size](https://towardsdatascience.com/creating-the-perfect-connect-four-ai-bot-c165115557b0)\
Code Bullet: [Creating the PERFECT Connect 4 A.I.](https://www.youtube.com/watch?v=XRVA5PMSKKE&t=664s&ab_channel=CodeBullet)

## Connect 4 Background
In a game of Connect 4, the players choose a color and then take turns dropping colored tokens into a six-row, seven-column vertically suspended grid. Afterwards, the turn is passed to the opponent which does the same and the goal of the game is to connect 4 tiles in a row before the opponent is able to. 

There are `4,531,985,219,092` legal game states possible encountered during play. This translate to a huge number of positions that a A.I. has to visit before determining which move is the best move in any given game state. Thus, the name of the game is optimization, everything from the board representation and the algorithm used to find good positions needs to be as efficient as possible in order to find the best move especially when it is a early game state. 

## Bitboard representation of Game State
To efficiently determine the best move in Connect 4, we need to explore a vast number of game states. To make this process efficient, it's crucial that the validation of a column and the end-of-game checks are optimized for speed. Therefore, our solution is to use unsigned 64-bit integers (unsigned long long/uint64_t) to represent the board. The reason is due to bitwise operations being extremely fast in modern-day computers.

### Bitwise operations
Left/Right shift operators `<<` / `>>`: They are used to shift bits a certain number to the left and right respectively. They are extremely useful when it comes to setting up masks for checks. For example.\
`0b01110010 >> 3` results in `0b00001110`\
`0b01110010 << 4` results in `0b00100000`

AND operator: The position of a bit is only set to be `1` only if both bits are `1`.

We can think of this operation as a `filter`, if we were to set the position of the bits that we want to check to be `1`, the position will be set to `1` if the other number has that bit in that position as `1`
```
  10101110
& 00001111
----------
  00001110

In this example, we are checking if the least significant 4 bits are on
```
OR operator: The position of a bit is set to `1` if at least one of the bits in the position is `1`.

We can think of this operation as a `set`, if we were to set the position of the bits that we want on to be `1`, the output will be the number those bits turned on.
```
  10101110
| 00001111
----------
  10101111

In this example, we are setting least significant 4 bits to be on
```
XOR operator: The position of the bit is set to `1` if and only if a single bit in that position is `1`.\

This operation is useful if we want to "undo" a operation. This will be a key feature in our `move()` function.
```
  10101110
^ 00001111
----------
  10100001
```
### Board representation
To represent any given board state using a unsigned 64 bit integer, the bits representing each position where a tile can be on the board are shown in the diagram below. Note that a Sentinel serves a dual purpose in checking if a column is full and to ensure winning board state checks are correct.
```
  6 13 20 27 34 41 48   55 62     Sentinel Row
+---------------------+ 
| 5 12 19 26 33 40 47 | 54 61      
| 4 11 18 25 32 39 46 | 53 60
| 3 10 17 24 31 38 45 | 52 59
| 2  9 16 23 30 37 44 | 51 58
| 1  8 15 22 29 36 43 | 50 57
| 0  7 14 21 28 35 42 | 49 56 63  bottom row
+---------------------+
```
To represent a board_state, only two unsigned 64 bit integer are needed because by using bitwise operations, we can toggle between opponent and our current positon.

### Move function
So given this board state and it is `O`'s turn to play and the tile is placed on column `4`
```
board state                curr_pos mask              board_mask mask
. . . . . . .              . . . . . . .              . . . . . . .
. . . . . . .              . . . . . . .              . . . . . . .
. . . . . . .              . . . . . . .              . . . . . . .
. . . . . . .              . . . . . . .              . . . . . . .
. . . O X . .              . . . O . . .              . . . # # . .
. . O X X . .              . . O . . . .              . . # # # . .
-------------              -------------              -------------
0 1 2 3 4 5 6              0 1 2 3 4 5 6              0 1 2 3 4 5 6

the bits at position 14, 21, 22, 28, 29 are on for board_mask 
```
We will first toggle `curr_pos` mask using bitwise `XOR` before peforming bitwise `OR` on the `board_mask` to play the move, this results in the following changes to the two bitmasks
```
board state                curr_pos mask              board_mask mask
. . . . . . .              . . . . . . .              . . . . . . .
. . . . . . .              . . . . . . .              . . . . . . .
. . . . . . .              . . . . . . .              . . . . . . .
. . . . O . .              . . . . . . .              . . . . # . .
. . . O X . .              . . . . X . .              . . . # # . .
. . O X X . .              . . . X X . .              . . # # # . .
-------------              -------------              -------------
0 1 2 3 4 5 6              0 1 2 3 4 5 6              0 1 2 3 4 5 6

We are passing the turn over to our opponent (the opponent's position mask remains unchanged). Afterwards we play our tile in column 4
```
As some of you may be wondering, how does one get the bit position of the bottom most unoccupied column to turn on the bit in that column? 

Note that in each column, the bits always represents a binary number that is `(2^n)-1` that is shifted `col*(NROWS+1)` to that position. So by adding a bit at the bottom row's position corresponding to the column that we want to place results in as mask with a single bit that represents our move to the bitboard. Using a column mask and a sentinel row check, we are able to check if it is valid move quickly.

With this, obtaining the mask for all possible moves follows the same logic:
```
board mask                 curr_pos mask              FULL_BOARD_MASK            possible_move mask
. . . . . . .              . . . . . . .              . . . . . . .              . . . . . . .
. . . . . . .              . . . . . . .              # # # # # # #              . . . . . . .
. . . . . . .              . . . . . . .              # # # # # # #              . . . . # . .
. . . . # . .       +      . . . . . . .       &      # # # # # # #       =      . . . # . . .
. . . # # . .              . . . . . . .              # # # # # # #              . . # . . . .
. . # # # . .              # # # # # # #              # # # # # # #              # # . . . # #
-------------              -------------              -------------              -------------
0 1 2 3 4 5 6              0 1 2 3 4 5 6              0 1 2 3 4 5 6              0 1 2 3 4 5 6

(board_mask + BOTTOM_MASK) & FULL_BOARD_MASK -> possible_move mask
```
### Winning Moves Check
Now we are getting into the reason why we are using bitboards to represent the board. Since we need to check for four in a row almost everytime a move is made, it is extremely important that the checks done are as efficient as possible. Now lets revisit back to the position encodings for the bitboard.
```
  6 13 20 27 34 41 48   55 62     Sentinel Row
+---------------------+ 
| 5 12 19 26 33 40 47 | 54 61      
| 4 11 18 25 32 39 46 | 53 60
| 3 10 17 24 31 38 45 | 52 59
| 2  9 16 23 30 37 44 | 51 58
| 1  8 15 22 29 36 43 | 50 57
| 0  7 14 21 28 35 42 | 49 56 63  bottom row
+---------------------+
```
Notice that to check for 4 in a row in the direction that we are interested in, they are being offset by a fix number of bits. As such, we are given this formula to check for winning moves 
```cpp
// dir must be one of the following values for the different winning checks
// dir == 1 (vertical check)
// dir == 7 (horizontal check)
// dir == 8 (diagonal-right check)
// dir == 6 (diagonal-left check)
bool is_game_over(uint64_t board, int dir) {
    return board & (board >> dir) & (board >> 2*dir) & (board >> 3*dir)
}
```
Where `dir` is the difference in offset that we are interested in. Imagine we are trying to layer different positions of a arbitrary position encoding with certain specific offsets to check for 4 in a row. As long as one of the position is not `1` the function will return `false`. This allows us to check for all position encodings with simple bitshifts operations as efficiently as possible.

## Negamax Algorithm
I'll write some time later (bitboard took a super long time)
