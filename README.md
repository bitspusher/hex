# Build Executable

 `make`
 
# Run the program
`./hex`

# Player Selection
Enter `1` to go first, `2` otherwise
 
# Make a Move
To place B/R at 0,0: `0` `0`

# Rules
Player 1 (Blue) needs to make a continuous path from top to bottom to win. Similarly Player 2(Red) needs to create a path from Left to Right.

# Code Layout

`main()` is implemented in `hex.cpp` It contains high-level logic for user input/output.

The files `hex_board.h` and `hex_board.cpp` contain functionality for initializing,
evaluating status, and move-making/checking.

The files `hex_monte_carlo.cpp` and `hex_monte_carlo.h` contain functionality for
evaluating the AI's best move.
