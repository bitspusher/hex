#include "hex_board.h"

#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

/* Constructor for a cell in the Hex Grid. All cells start out empty,
and should have a distinct x,y position */
HexCell::HexCell(int rowIdx, int colIdx) : rowIdx_(rowIdx), colIdx_(colIdx) {
  id_ = '.';
  marked_ = false;
}

// Indicates if this cell hasn't been claimed by a player.
bool HexCell::isEmpty(void) const { return id_ == '.' ? true : false; }

void HexCell::displayValue(void) const { std::cout << id_; }

int HexCell::getRow(void) const { return rowIdx_; }

int HexCell::getCol(void) const { return colIdx_; }

char HexCell::getValue(void) const { return id_; }

void HexCell::setValue(char val) { id_ = val; }

/* Helper functions for performing a BFS.
Used to indicate that a node was already 'visited'.*/
void HexCell::mark(void) { marked_ = true; }

bool HexCell::isMarked(void) const { return (marked_ == true); }

void HexCell::clearMark(void) { marked_ = false; }

HexBoard::HexBoard(int numRows, int numCols, HexPlayer ai, HexPlayer human)
    : numRows_(numRows), numCols_(numCols), ai_(ai), human_(human) {
  rows_.resize(numRows_);
  state_ = HexGameState::INIT;

  for (auto rowId = 0; rowId < numRows_; rowId++) {
    for (auto colId = 0; colId < numRows_; colId++) {
      rows_[rowId].push_back(HexCell(rowId, colId));
    }
  }
  state_ = HexGameState::WAIT_FOR_PLAYER_BLUE_INPUT;
}

/* Displays the contents of the HEX board */
void HexBoard::display(void) {
  int numPrecedingSpaces = 0;

  // Helpers for display formatting specific to end of row, or end of grid
  int curColIdx = 0;
  int curRowIdx = 0;

  // Print Column IDs for ease of move selection
  std::cout << "  0";
  for (auto i = 1; i < numCols_; i++) {
    std::cout << "   " << i;
  }
  std::cout << std::endl;
  numPrecedingSpaces++;

  // Traverse the grid row-by-row.
  for (auto &row : rows_) {
    // Print row IDs for ease of move selection
    std::cout << curRowIdx;
    auto spacesBetweenRowIDAndFirstCell = (curRowIdx >= 10) ? (numPrecedingSpaces-1) : numPrecedingSpaces;
    
    for (auto i = 0; i < spacesBetweenRowIDAndFirstCell; ++i) {
      std::cout << ' ';
    }
    
    numPrecedingSpaces++;
    curColIdx = 0;
    for (auto &elem : row) {
      elem.displayValue();

      if (curColIdx != row.size() - 1) {
        std::cout << " - ";
      }
      curColIdx++;
    }
    std::cout << std::endl;
    
    numPrecedingSpaces++;
    for (auto i = 0; i < numPrecedingSpaces; ++i) {
      std::cout << ' ';
    }

    curColIdx = 0;
    if (curRowIdx != rows_.size() - 1) {
      for (auto i = 0; i < row.size(); i++) {
        std::cout << "\\ ";

        if (curColIdx != row.size() - 1) {
          std::cout << "/ ";
        }
        curColIdx++;
      }
    }
    std::cout << std::endl;
    curRowIdx++;
  }
}

/* Handles simple state_machine transitions*/
void HexBoard::switchState(void) {
  switch (state_) {
  case HexGameState::WAIT_FOR_PLAYER_BLUE_INPUT:
    state_ = HexGameState::WAIT_FOR_PLAYER_RED_INPUT;
    break;
  case HexGameState::WAIT_FOR_PLAYER_RED_INPUT:
    state_ = HexGameState::WAIT_FOR_PLAYER_BLUE_INPUT;
    break;
  default:
    break;
  }
}

/* Checks for a continuous path of the same color/player from any one
of the cells in the top-most row to any cell in the bottom row.*/
std::pair<bool, HexPlayer> HexBoard::checkIfGameHasEnded(void) {
  // BFS on Hex Grid to find a continuous path to any cell in the last row
  // starting from any cell in the first row.

  // Queue to store cells to visit while performing the path search.
  std::queue<HexCell *> hexCellQ;

  // Set to true if a path was found
  bool gameEnded = false;

  std::vector<HexCell> startCells;

  // The possible start cells for a complete path are on the first row or
  // first column
  for (auto &startCell : rows_[0]) {
    startCells.push_back(startCell);
  }
  for (auto &row : rows_) {
    auto startCell = row[0];
    startCells.push_back(startCell);
  }

  // Start search for path to an ending cell(bottom row/last column) from any
  // one of the starting cells.
  bool searchingTopToBottom = true;

  for (auto &startCell : startCells) {
    char playerId = startCell.getValue();

    if (startCell.isEmpty() == true) {
      // This particular cell hasn't been claimed yet, no path
      // to search for this cell, so move to next adjacent cell.
      continue;
    }

    searchingTopToBottom = (playerId == 'B') ? true : false;

    // Clear the contents of the queue.
    std::queue<HexCell *> empty;
    std::swap(hexCellQ, empty);

    // Start the search by inserting the first node into the queue of nodes to
    // visit
    startCell.mark();
    hexCellQ.push(&startCell);

    while (!hexCellQ.empty() && !gameEnded) {
      HexCell *curNode = hexCellQ.front();
      hexCellQ.pop();
      std::vector<HexCell *> neighbors = getNeighbors(*curNode);

      // Add neighbors to list of cells to visit
      for (auto &n : neighbors) {
        if ((n->getValue() == playerId) && (n->isMarked() == false)) {
          n->mark();
          hexCellQ.push(n);

          if (((n->getRow() == numRows_ - 1) && (searchingTopToBottom)) ||
              ((n->getCol() == numCols_ - 1) && (!searchingTopToBottom))) {
            gameEnded = true;
            winner_ = (playerId == 'B') ? HexPlayer::Blue : HexPlayer::Red;
            state_ = HexGameState::COMPLETED;
            break;
          }
        }
      }
    }

    // Reset the flags used for BFS
    clearAllMarkedFlags();

    if (gameEnded) {
      break;
    }
  }
  return std::make_pair(gameEnded, winner_);
}

HexPlayer HexBoard::getWinner(void) const { return winner_; }

void HexBoard::makeMove(const int rowIdx, const int colIdx,
                        const HexPlayer player) {
  rows_[rowIdx][colIdx].setValue((player == HexPlayer::Blue) ? 'B' : 'R');
}

/* Gets input from the user or AI in row, col format and checks for validity
before updating the grid */
HexGameState HexBoard::getInput(void) {
  int rowIdx, colIdx;
  HexPlayer player = (state_ == HexGameState::WAIT_FOR_PLAYER_BLUE_INPUT)
                         ? HexPlayer::Blue
                         : HexPlayer::Red;
  HexMonteCarlo mc(this, HexPlayer::Blue, HexPlayer::Red);

  std::cout << ((player == HexPlayer::Blue) ? "Blue's turn" : "Red's turn")
            << std::endl;

  if ((state_ == HexGameState::WAIT_FOR_PLAYER_BLUE_INPUT &&
       ai_ == HexPlayer::Blue) ||
      (state_ == HexGameState::WAIT_FOR_PLAYER_RED_INPUT &&
       ai_ == HexPlayer::Red)) {
    HexCell bestMove = mc.getBestMove();
    rowIdx = bestMove.getRow();
    colIdx = bestMove.getCol();
  } else {
    std::cin >> rowIdx >> colIdx;
  }

  if ((rowIdx >= numRows_) || (colIdx >= numCols_)) {
    std::cout << "Out of bounds" << std::endl;
  } else if (!rows_[rowIdx][colIdx].isEmpty()) {
    std::cout << "Invalid cell entered!" << std::endl;
  } else {
    makeMove(rowIdx, colIdx, player);
    switchState();
  }
  checkIfGameHasEnded();
  return state_;
}

/* Given a cell in the Hex grid, returns a list of its neighbors*/
std::vector<HexCell *> HexBoard::getNeighbors(HexCell cell) {
  std::vector<HexCell *> neighbors;
  int rowId = cell.getRow();
  int colId = cell.getCol();

  // Simplest case
  if (isEdgeNode(cell) == false) {
    neighbors.push_back(&rows_[rowId - 1][colId]);
    neighbors.push_back(&rows_[rowId - 1][colId + 1]);
    neighbors.push_back(&rows_[rowId][colId + 1]);
    neighbors.push_back(&rows_[rowId][colId - 1]);
    neighbors.push_back(&rows_[rowId + 1][colId]);
    neighbors.push_back(&rows_[rowId + 1][colId - 1]);
  } else {
    // 0,0
    if ((rowId == 0) && (colId == 0)) {
      neighbors.push_back(&rows_[1][0]);
      neighbors.push_back(&rows_[0][1]);
    } else if ((rowId == numRows_ - 1) && (colId == numCols_ - 1)) {
      neighbors.push_back(&rows_[numRows_ - 2][numCols_ - 1]);
      neighbors.push_back(&rows_[numRows_ - 1][numCols_ - 2]);
    } else if ((rowId == 0) && (colId == numCols_ - 1)) {
      neighbors.push_back(&rows_[0][numCols_ - 2]);
      neighbors.push_back(&rows_[1][numCols_ - 2]);
      neighbors.push_back(&rows_[1][numCols_ - 1]);
    } else if ((rowId == numRows_ - 1) && (colId == 0)) {
      neighbors.push_back(&rows_[numRows_ - 2][0]);
      neighbors.push_back(&rows_[numRows_ - 2][1]);
      neighbors.push_back(&rows_[numRows_ - 1][1]);
    } else if (rowId == 0) {
      neighbors.push_back(&rows_[0][colId - 1]);
      neighbors.push_back(&rows_[0][colId + 1]);
      neighbors.push_back(&rows_[1][colId - 1]);
      neighbors.push_back(&rows_[1][colId]);
    } else if (colId == 0) {
      neighbors.push_back(&rows_[rowId - 1][colId + 1]);
      neighbors.push_back(&rows_[rowId - 1][colId]);
      neighbors.push_back(&rows_[rowId][colId + 1]);
      neighbors.push_back(&rows_[rowId + 1][colId]);
    } else if (rowId == numRows_ - 1) {
      neighbors.push_back(&rows_[numRows_ - 1][colId + 1]);
      neighbors.push_back(&rows_[numRows_ - 1][colId - 1]);
      neighbors.push_back(&rows_[numRows_ - 2][colId]);
      neighbors.push_back(&rows_[numRows_ - 2][colId + 1]);
    } else if (colId == numCols_ - 1) {
      neighbors.push_back(&rows_[rowId - 1][colId]);
      neighbors.push_back(&rows_[rowId][colId - 1]);
      neighbors.push_back(&rows_[rowId + 1][colId - 1]);
      neighbors.push_back(&rows_[rowId + 1][colId]);
    }
  }
  return neighbors;
}

/* Resets mark/visited flag used during a BFS*/
void HexBoard::clearAllMarkedFlags(void) {
  for (auto &row : rows_) {
    for (auto &cell : row) {
      cell.clearMark();
    }
  }
}

/* Helper function used in position-specific handling of a cell */
bool HexBoard::isEdgeNode(HexCell cell) {
  int row = cell.getRow();
  int col = cell.getCol();

  if ((row == 0) || (row == numRows_ - 1) || (col == 0) ||
      (col == numCols_ - 1)) {
    return true;
  } else {
    return false;
  }
}
