Instructions to run the program

 1. Compile: make
 2. Run:     ./hex
 3. Player Selection: To go first; 1 (2 otherwise)
 3. Interact: Place X at 0,0: 0 0
 4. AI makes a move
 5. ... Repeat 3,4 with unique x,y input values until game is completed

Code Layout

main() is implemented in hex.cpp. It contains high-level logic for user input/output.

The files hex_board.h and hex_board.cpp contain functionality for initializing,
evaluating status, and move-making/checking.

The files hex_monte_carlo.cpp and hex_monte_carlo.h contain functionality for
evaluating the next best move by passing in a HexBoard object.


Comments

The APIs have comment blocks in the header files.
