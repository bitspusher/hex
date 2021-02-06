/*
Hex Program

AUTHOR: Sanjay DMello
DATE: 2/1/2021

Contains functionality to initialize a HEX board, receive input from
the command-line to populate a cell, and then display the state/contents of the grid.

The user is allowed to go first if desired. The AI uses the Monte Carlo method
to evaluate the best move in a reasonable amount of time.

 Example usage:
 1. Compile: make
 2. Run:     ./hex
 3. Player selection: Enter 1 to play as Player 1/Blue.
 4. Size selection: Enter 11 for a 11x11 grid
 5. Interact:  0 0 to place 'B' at (0,0)
 6. AI makes a move
 7. Repeat 5 and 6 till game completion
...
*/
#include <iostream>
#include <queue>
#include <vector>

#include "hex_board.h"
/*
  Initializes the HEX Grid, makes a move, waits for user-input, and displays
  the updated grid after each move.
*/
int main(int argc, char *argv[]) {
  HexPlayer ai, human;

  std::cout << "Starting Hex Game" << std::endl;
  std::cout << "*****************************" << std::endl;
  std::cout << "Top and Bottom sides are BLUE" << std::endl;
  std::cout << "Left and Right sides are RED" << std::endl;
  std::cout << "*****************************" << std::endl;

  while (1) {
    int player_selection;

    std::cout << "Enter 1 to play as Player 1 (Blue) or 2 for Player 2(Red)"
              << std::endl;
    std::cin >> player_selection;

    if (player_selection == 1) {
      ai = HexPlayer::Red;
      human = HexPlayer::Blue;
      break;
    } else if (player_selection == 2) {
      ai = HexPlayer::Blue;
      human = HexPlayer::Red;
      break;
    } else {
      std::cout << "Invalid selection! Try again" << std::endl;
    }
  }

  int size;
  while (1) {
    std::cout << "Enter size of board" << std::endl;
    std::cin >> size;

    if ((size > 11) || (size < 3)) {
      std::cout << "Size of board should be between 3 and 11." << std::endl;
    } else {
      break;
    }
  }

  HexBoard board(size, size, ai, human);
  board.display();

  while (board.getInput() != HexGameState::COMPLETED) {
    board.display();
  }
  board.display();
  auto winner = board.getWinner();
  std::cout << "Player " << ((winner == HexPlayer::Blue) ? "Blue" : "Red")
            << " has won the game!" << std::endl;
  return 0;
}
