#pragma once

#include "hex_monte_carlo.h"
#include <vector>

// Forward declarations
class HexMonteCarlo;

/* Represents a cell in the Hex Board Grid. Each cell
has identifiers to indicate its position(x,y) and value(Empty, PlayerA,
PlayerB)*/
class HexCell {
private:
  char id_;
  int rowIdx_, colIdx_;
  bool marked_;

public:
  /* Constructor for a cell in the Hex Grid. All cells start out empty,
  and should have a distinct x,y position */
  HexCell(int rowIdx = 0, int colIdx = 0);
  // Indicates if this cell hasn't been claimed by a player.
  bool isEmpty(void) const;
  void displayValue(void) const;
  int getRow(void) const;
  int getCol(void) const;
  char getValue(void) const;
  void setValue(char val);
  /* Helper functions for performing a BFS.
  Used to indicate that a node was already 'visited'.*/
  void mark(void);
  bool isMarked(void) const;
  void clearMark(void);
};

// Each row in the grid is just a vector of Hex Cells.
typedef std::vector<HexCell> hexRow;

/* States used to represent the Hex Game State Machine*/
enum class HexGameState {
  INIT = 0,
  WAIT_FOR_PLAYER_BLUE_INPUT,
  WAIT_FOR_PLAYER_RED_INPUT,
  COMPLETED,
};

enum class HexPlayer { Blue, Red };

/*
  This class provides functionality for creating the Hex Board, displaying
  its contents with formatting, enforcing the rules of the game, and determines
  the winner, if any.
*/
class HexBoard {
  friend class HexMonteCarlo;

private:
  // Member variables
  std::vector<hexRow> rows_;
  int numRows_, numCols_;
  HexGameState state_;
  HexPlayer winner_;
  HexPlayer ai_, human_;

  // Member functions
  /* Handles simple state machine transitions*/
  void switchState(void);

  /* Checks for a continuous path of the same color/player from any one
  of the cells in the top-most row to any cell in the bottom row.*/
  std::pair<bool, HexPlayer> checkIfGameHasEnded(void);

  void makeMove(const int rowIdx, const int colIdx, const HexPlayer player);

  /* Given a cell in the Hex grid, returns a list of its neighbors*/
  std::vector<HexCell *> getNeighbors(HexCell cell);

  /* Resets mark/visited flag used during a BFS*/
  void clearAllMarkedFlags(void);

  /* Helper function used in position-specific handling of a cell */
  bool isEdgeNode(HexCell cell);

public:
  /* Constructor. Initializes the HEX Board's cells*/
  HexBoard(int numRows, int numCols, HexPlayer ai, HexPlayer human);

  /* Displays the contents of the HEX board */
  void display(void);

  HexPlayer getWinner(void) const;

  /* Gets input from the user or AI in row, col format and checks for validity
  before updating the grid */
  HexGameState getInput(void);
};
